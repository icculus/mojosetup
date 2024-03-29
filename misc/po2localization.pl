#!/usr/bin/perl -w

use warnings;
use strict;
use Encode qw( decode_utf8 );

# Fixes unicode dumping to stdio...hopefully you have a utf-8 terminal by now.
binmode(STDIN, ":utf8");
binmode(STDOUT, ":utf8");
binmode(STDERR, ":utf8");

my $now = `date '+%Y-%m-%d %H:%M:%S%z'`;
chomp($now);

my $gitver = `git describe --always --dirty 2>/dev/null`;
$gitver = '???' if ($gitver eq '');

my %languages;
my %comments;
my %msgstrs;
my @strings;
my $saw_template = 0;
my $exportdate = '';
my $generator = '';

my $app_mode;
if ($ARGV[0] eq "--app")
{
    shift @ARGV;
    $app_mode=1
}

foreach (@ARGV) {
    my $fname = $_;
    my $template = /\.pot\Z/;

    open(POIO, '<', $fname) or die("Failed to open $_: $!\n");
    binmode(POIO, ":utf8");

    if ($template) {
        die("multiple .pot files specified\n") if ($saw_template);
        $saw_template = 1;
    }

    my $comment = '';
    my $currentlang = '';
    my $fuzzy;

    while (<POIO>) {
        chomp;
        s/\A\s+//;
        s/\s+\Z//;
        next if ($_ eq '');

        if (s/\A\#\.\s*(.*)\Z/$1/) {
            if ($template) {
                my $txt = $_;
                $txt = " $txt" if ($comment ne '');
                $comment .= "    -- $txt\n";
            }
            next;
        }
        if (/\A\#,.*\bfuzzy\b/) {
            $fuzzy = 1;
            next;
        }

        next if /\A\#/;

        if (s/msgid\s*\"(.*?)\"\Z/$1/) {
            if (($_ eq '') and ($currentlang eq '')) {   # initial string.
                # Provide a default value in case the 'Language' field is
                # missing
                $currentlang=$1 if ($fname =~ /\b([a-z]{2,3}(?:_[A-Z]{2})?)\.po$/);
                my $langname;
                while (<POIO>) {  # Skip most of the metadata.
                    chomp;
                    s/\A\s+//;
                    s/\s+\Z//;
                    last if ($_ eq '');
                    if (!$currentlang and /\A\"Language: ([a-z]{2,3}(?:_[A-Z]{2})?)\\n"\Z/) {
                        $currentlang = $1;
                    } elsif (/\A\"Language-Team: (.*?) <.*?>\\n"\Z/) {
                        $langname = $1;
                    } elsif (s/\A\"(X-Launchpad-Export-Date: .*?)\\n\"/$1/) {
                        $exportdate = $_ if ($template);
                    } elsif (s/\A"(X-Generator: .*?)\\n\"\Z/$1/) {
                        $generator = $_ if ($template);
                    }
                }
                if (not $template) {
                    if ($currentlang eq '') {
                        die("could not determine the language of '$fname'\n");
                    } elsif (exists $languages{$currentlang}) {
                        die("Same language twice: $currentlang\n");
                    } elsif ($currentlang eq 'en') {
                        die("Found an 'en' translation.\n");
                    } elsif ($currentlang eq 'en_US') {
                        die("Found an 'en_US' translation.\n");
                    }
                    $languages{$currentlang} = $langname;
                }
            } elsif ($currentlang eq '' and not $template) {
                die("No current language!\n");
            } else {  # new string
                my $msgstr = '';
                my $msgid = $_;
                while (<POIO>) {   # check for multiline msgid strings.
                    chomp;
                    s/\A\s+//;
                    s/\s+\Z//;
                    if (s/\Amsgstr \"(.*?)\"\Z/$1/) {
                        $msgstr = $_;
                        last;
                    }
                    if (s/\A\"(.*?)\"\Z/$1/) {
                        $msgid .= $_;
                    } else {
                        die("unexpected line: $_\n");
                    }
                }
                while (<POIO>) {   # check for multiline msgstr strings.
                    chomp;
                    s/\A\s+//;
                    s/\s+\Z//;
                    last if ($_ eq '');
                    if (s/\A\"(.*?)\"\Z/$1/) {
                        $msgstr .= $_;
                    } else {
                        die("unexpected line: $_\n");
                    }
                }
                $msgstr = '' if ($fuzzy);

                if ($template) {
                    push @strings, $msgid;  # This is a list, to keep original order.
                    $comments{$msgid} = $comment;
                    $comment = '';
                } elsif ($msgstr ne '') {
                    $msgstrs{$currentlang}{$msgid} = $msgstr;
                }
            }
            $fuzzy = undef;
        }
    }

    close(POIO);
}

die("no template seen\n") if (not $saw_template);


if ($app_mode)
{
    print <<__EOF__;
-- DO NOT EDIT BY HAND.
-- This file was generated with po2localization.pl, version $gitver ...
--  on $now

__EOF__
}
else
{
    print <<__EOF__;
-- MojoSetup; a portable, flexible installation application.
--
-- Please see the file LICENSE.txt in the source's root directory.
--
-- DO NOT EDIT BY HAND.
-- This file was generated with po2localization.pl, version $gitver ...
--  on $now
--
-- Your own installer's localizations go into app_localization.lua instead.
-- If you want to add strings to be translated to this file, contact Ryan
-- (icculus\@icculus.org). If you want to add or change a translation for
-- existing strings, please use our nice web interface here for your work:
--
--    https://translations.launchpad.net/mojosetup/
--
-- ...and that work eventually ends up in this file.
--
-- $exportdate
-- $generator

MojoSetup.languages = {
__EOF__
    print "    en_US = \"English (United States)\"";

    foreach (sort keys %languages) {
        my $k = $_;
        my $v = $languages{$k};
        print ",\n    $k = \"$v\""
    }
    print "\n};\n\n";
}

if ($app_mode)
{
    print "MojoSetup.applocalization = {";
}
else
{
    print "MojoSetup.localization = {";
}

foreach (@strings) {
    my $msgid = $_;
    print "\n";
    print $comments{$msgid};
    print "    [\"$msgid\"] = {\n";
    my $first = 1;
    foreach (sort keys %languages) {
        my $k = $_;
        my $str = $msgstrs{$k}{$msgid};
        next if ((not defined $str) or ($str eq ''));
        print ",\n" if (not $first);
        print "        $k = \"$str\"";
        $first = 0;
    }
    print "\n    };\n";
}

print "};\n";
print "\n-- end of localization.lua ...\n" if (not $app_mode);

# end of po2localization.pl ...

