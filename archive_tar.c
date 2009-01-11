/**
 * MojoSetup; a portable, flexible installation application.
 *
 * Please see the file LICENSE.txt in the source's root directory.
 *
 *  This file written by Ryan C. Gordon.
 */

// Specs for the tar format can be found here...
//   http://www.gnu.org/software/tar/manual/html_section/Standard.html

#include "fileio.h"

#if !SUPPORT_TAR
MojoArchive *MojoArchive_createTAR(MojoInput *io) { return NULL; }
#else

#if SUPPORT_TAR_GZ

#include "zlib-1.2.3/zlib.h"

#define GZIP_READBUFSIZE (128 * 1024)

typedef struct GZIPinfo
{
    MojoInput *origio;
    uint64 uncompressed_position;
    uint8 *buffer;
    z_stream stream;
} GZIPinfo;


static MojoInput *make_gzip_input(MojoInput *origio);

static voidpf mojoZlibAlloc(voidpf opaque, uInt items, uInt size)
{
    return xmalloc(items * size);
} // mojoZlibAlloc

static void mojoZlibFree(voidpf opaque, voidpf address)
{
    free(address);
} // mojoZlibFree

static void initializeZStream(z_stream *pstr)
{
    memset(pstr, '\0', sizeof (z_stream));
    pstr->zalloc = mojoZlibAlloc;
    pstr->zfree = mojoZlibFree;
} // initializeZStream

static boolean MojoInput_gzip_ready(MojoInput *io)
{
    return true;
} // MojoInput_gzip_ready

static boolean MojoInput_gzip_seek(MojoInput *io, uint64 offset)
{
    // This is all really expensive.
    GZIPinfo *info = (GZIPinfo *) io->opaque;

    /*
     * If seeking backwards, we need to redecode the file
     *  from the start and throw away the compressed bits until we hit
     *  the offset we need. If seeking forward, we still need to
     *  decode, but we don't rewind first.
     */
    if (offset < info->uncompressed_position)
    {
        if (!info->origio->seek(info->origio, 0))
            return false;
        inflateEnd(&info->stream);
        initializeZStream(&info->stream);
        if (inflateInit2(&info->stream, 31) != Z_OK)
            return false;
        info->uncompressed_position = 0;
    } // if

    while (info->uncompressed_position != offset)
    {
        uint8 buf[512];
        uint32 maxread;
        int64 br;

        maxread = (uint32) (offset - info->uncompressed_position);
        if (maxread > sizeof (buf))
            maxread = sizeof (buf);

        br = io->read(io, buf, maxread);
        if (br != maxread)
            return false;
    } /* while */

    return true;
} // MojoInput_gzip_seek

static int64 MojoInput_gzip_tell(MojoInput *io)
{
    return (((GZIPinfo *) io->opaque)->uncompressed_position);
} // MojoInput_gzip_tell

static int64 MojoInput_gzip_length(MojoInput *io)
{
    return -1;
} // MojoInput_gzip_length

static int64 MojoInput_gzip_read(MojoInput *io, void *buf, uint32 bufsize)
{
    GZIPinfo *info = (GZIPinfo *) io->opaque;
    MojoInput *origio = info->origio;
    int64 retval = 0;

    if (bufsize == 0)
        return 0;    // quick rejection.
    else
    {
        info->stream.next_out = buf;
        info->stream.avail_out = bufsize;

        while (retval < ((int64) bufsize))
        {
            const uint32 before = info->stream.total_out;
            int rc;

            if (info->stream.avail_in == 0)
            {
                int64 br;

                br = origio->length(origio) - origio->tell(origio);
                if (br > 0)
                {
                    if (br > GZIP_READBUFSIZE)
                        br = GZIP_READBUFSIZE;

                    br = origio->read(origio, info->buffer, (uint32) br);
                    if (br <= 0)
                        return -1;

                    info->stream.next_in = info->buffer;
                    info->stream.avail_in = (uint32) br;
                } // if
            } // if

            rc = inflate(&info->stream, Z_SYNC_FLUSH);
            retval += (info->stream.total_out - before);

            if (rc != Z_OK)
                return -1;
        } // while
    } // else

    if (retval > 0)
        info->uncompressed_position += (uint32) retval;

    return retval;
} // MojoInput_gzip_read

static MojoInput* MojoInput_gzip_duplicate(MojoInput *io)
{
    GZIPinfo *info = (GZIPinfo *) io->opaque;
    MojoInput *retval = NULL;
    MojoInput *newio = info->origio->duplicate(info->origio);
    if (newio != NULL)
    {
        retval = make_gzip_input(newio);
        if (retval != NULL)
            retval->seek(retval, io->tell(io));  // slow, slow, slow...
    } // if
    return retval;
} // MojoInput_gzip_duplicate

static void free_gzip_input(MojoInput *io)
{
    GZIPinfo *info = (GZIPinfo *) io->opaque;
    inflateEnd(&info->stream);
    free(info->buffer);
    free(info);
    free(io);
} // free_gzip_input

static void MojoInput_gzip_close(MojoInput *io)
{
    GZIPinfo *info = (GZIPinfo *) io->opaque;
    if (info->origio != NULL)
        info->origio->close(info->origio);
    free_gzip_input(io);
} // MojoInput_gzip_close

static MojoInput *make_gzip_input(MojoInput *origio)
{
    MojoInput *io = NULL;
    GZIPinfo *info = (GZIPinfo *) xmalloc(sizeof (GZIPinfo));

    initializeZStream(&info->stream);
    if (inflateInit2(&info->stream, 31) != Z_OK)
    {
        free(info);
        return NULL;
    } // if

    info->origio = origio;
    info->buffer = (uint8 *) xmalloc(GZIP_READBUFSIZE);

    io = (MojoInput *) xmalloc(sizeof (MojoInput));
    io->ready = MojoInput_gzip_ready;
    io->read = MojoInput_gzip_read;
    io->seek = MojoInput_gzip_seek;
    io->tell = MojoInput_gzip_tell;
    io->length = MojoInput_gzip_length;
    io->duplicate = MojoInput_gzip_duplicate;
    io->close = MojoInput_gzip_close;
    io->opaque = info;
    return io;
} // make_gzip_info

#endif  // SUPPORT_TAR_GZ


#if SUPPORT_TAR_BZ2

#include "bzip2-1.0.4/bzlib.h"

#define BZIP2_READBUFSIZE (128 * 1024)

typedef struct BZIP2info
{
    MojoInput *origio;
    uint64 uncompressed_position;
    uint8 *buffer;
    bz_stream stream;
} BZIP2info;


static MojoInput *make_bzip2_input(MojoInput *origio);

static void *mojoBzlib2Alloc(void *opaque, int items, int size)
{
    return xmalloc(items * size);
} // mojoBzlib2Alloc

static void mojoBzlib2Free(void *opaque, void *address)
{
    free(address);
} // mojoBzlib2Free

static void initializeBZ2Stream(bz_stream *pstr)
{
    memset(pstr, '\0', sizeof (bz_stream));
    pstr->bzalloc = mojoBzlib2Alloc;
    pstr->bzfree = mojoBzlib2Free;
} // initializeBZ2Stream

static boolean MojoInput_bzip2_ready(MojoInput *io)
{
    return true;
} // MojoInput_bzip2_ready

static boolean MojoInput_bzip2_seek(MojoInput *io, uint64 offset)
{
    // This is all really expensive.
    BZIP2info *info = (BZIP2info *) io->opaque;

    /*
     * If seeking backwards, we need to redecode the file
     *  from the start and throw away the compressed bits until we hit
     *  the offset we need. If seeking forward, we still need to
     *  decode, but we don't rewind first.
     */
    if (offset < info->uncompressed_position)
    {
#if 0
        /* we do a copy so state is sane if inflateInit2() fails. */
        bz_stream str;
        initializeBZ2Stream(&str);
        if (BZ2_bzDecompressInit(&str, 0, 0) != BZ_OK)
            return false;

        if (!info->origio->seek(info->origio, 0))
            return false;  // !!! FIXME: leaking (str)?

        BZ2_bzDecompressEnd(&info->stream);
        memcpy(&info->stream, &str, sizeof (bz_stream));
#endif

        if (!info->origio->seek(info->origio, 0))
            return false;
        BZ2_bzDecompressEnd(&info->stream);
        initializeBZ2Stream(&info->stream);
        if (BZ2_bzDecompressInit(&info->stream, 0, 0) != BZ_OK)
            return false;
        info->uncompressed_position = 0;
    } // if

    while (info->uncompressed_position != offset)
    {
        uint8 buf[512];
        uint32 maxread;
        int64 br;

        maxread = (uint32) (offset - info->uncompressed_position);
        if (maxread > sizeof (buf))
            maxread = sizeof (buf);

        br = io->read(io, buf, maxread);
        if (br != maxread)
            return false;
    } /* while */

    return true;
} // MojoInput_bzip2_seek

static int64 MojoInput_bzip2_tell(MojoInput *io)
{
    return (((BZIP2info *) io->opaque)->uncompressed_position);
} // MojoInput_bzip2_tell

static int64 MojoInput_bzip2_length(MojoInput *io)
{
    return -1;
} // MojoInput_bzip2_length

static int64 MojoInput_bzip2_read(MojoInput *io, void *buf, uint32 bufsize)
{
    BZIP2info *info = (BZIP2info *) io->opaque;
    MojoInput *origio = info->origio;
    int64 retval = 0;

    if (bufsize == 0)
        return 0;    // quick rejection.
    else
    {
        info->stream.next_out = buf;
        info->stream.avail_out = bufsize;

        while (retval < ((int64) bufsize))
        {
            const uint32 before = info->stream.total_out_lo32;
            int rc;

            if (info->stream.avail_in == 0)
            {
                int64 br;

                br = origio->length(origio) - origio->tell(origio);
                if (br > 0)
                {
                    if (br > BZIP2_READBUFSIZE)
                        br = BZIP2_READBUFSIZE;

                    br = origio->read(origio, info->buffer, (uint32) br);
                    if (br <= 0)
                        return -1;

                    info->stream.next_in = (char *) info->buffer;
                    info->stream.avail_in = (uint32) br;
                } // if
            } // if

            rc = BZ2_bzDecompress(&info->stream);
            retval += (info->stream.total_out_lo32 - before);
            if (rc != BZ_OK)
                return -1;
        } // while
    } // else

    if (retval > 0)
        info->uncompressed_position += (uint32) retval;

    return retval;
} // MojoInput_bzip2_read

static MojoInput* MojoInput_bzip2_duplicate(MojoInput *io)
{
    BZIP2info *info = (BZIP2info *) io->opaque;
    MojoInput *retval = NULL;
    MojoInput *newio = info->origio->duplicate(info->origio);
    if (newio != NULL)
    {
        retval = make_bzip2_input(newio);
        if (retval != NULL)
            retval->seek(retval, io->tell(io));  // slow, slow, slow...
    } // if
    return retval;
} // MojoInput_bzip2_duplicate

static void free_bzip2_input(MojoInput *io)
{
    BZIP2info *info = (BZIP2info *) io->opaque;
    BZ2_bzDecompressEnd(&info->stream);
    free(info->buffer);
    free(info);
    free(io);
} // free_bzip2_input

static void MojoInput_bzip2_close(MojoInput *io)
{
    BZIP2info *info = (BZIP2info *) io->opaque;
    if (info->origio != NULL)
        info->origio->close(info->origio);
    free_bzip2_input(io);
} // MojoInput_bzip2_close

static MojoInput *make_bzip2_input(MojoInput *origio)
{
    MojoInput *io = NULL;
    BZIP2info *info = (BZIP2info *) xmalloc(sizeof (BZIP2info));

    initializeBZ2Stream(&info->stream);
    if (BZ2_bzDecompressInit(&info->stream, 0, 0) != BZ_OK)
    {
        free(info);
        return NULL;
    } // if

    info->origio = origio;
    info->buffer = (uint8 *) xmalloc(BZIP2_READBUFSIZE);

    io = (MojoInput *) xmalloc(sizeof (MojoInput));
    io->ready = MojoInput_bzip2_ready;
    io->read = MojoInput_bzip2_read;
    io->seek = MojoInput_bzip2_seek;
    io->tell = MojoInput_bzip2_tell;
    io->length = MojoInput_bzip2_length;
    io->duplicate = MojoInput_bzip2_duplicate;
    io->close = MojoInput_bzip2_close;
    io->opaque = info;
    return io;
} // make_bzip2_info

#endif  // SUPPORT_TAR_BZ2


// MojoInput implementation...

// Decompression is handled in the parent MojoInput, so this just needs to
//  make sure we stay within the bounds of the tarfile entry.

typedef struct TARinput
{
    int64 fsize;
    int64 offset;
    MojoArchive *ar;
} TARinput;

typedef struct TARinfo
{
    MojoInput *input;
    uint64 curFileStart;
    uint64 nextEnumPos;
} TARinfo;

static boolean MojoInput_tar_ready(MojoInput *io)
{
    return true;
} // MojoInput_tar_ready

static int64 MojoInput_tar_read(MojoInput *io, void *buf, uint32 bufsize)
{
    TARinput *input = (TARinput *) io->opaque;
    int64 pos = io->tell(io);
    if ((pos + bufsize) > input->fsize)
        bufsize = (uint32) (input->fsize - pos);
    return input->ar->io->read(input->ar->io, buf, bufsize);
} // MojoInput_tar_read

static boolean MojoInput_tar_seek(MojoInput *io, uint64 pos)
{
    TARinput *input = (TARinput *) io->opaque;
    boolean retval = false;
    if (pos < ((uint64) input->fsize))
        retval = input->ar->io->seek(input->ar->io, input->offset + pos);
    return retval;
} // MojoInput_tar_seek

static int64 MojoInput_tar_tell(MojoInput *io)
{
    TARinput *input = (TARinput *) io->opaque;
    return input->ar->io->tell(input->ar->io) - input->offset;
} // MojoInput_tar_tell

static int64 MojoInput_tar_length(MojoInput *io)
{
    return ((TARinput *) io->opaque)->fsize;
} // MojoInput_tar_length

static MojoInput *MojoInput_tar_duplicate(MojoInput *io)
{
    MojoInput *retval = NULL;
    fatal(_("BUG: Can't duplicate tar inputs"));  // !!! FIXME: why not?
#if 0
    TARinput *input = (TARinput *) io->opaque;
    MojoInput *origio = (MojoInput *) io->opaque;
    MojoInput *newio = origio->duplicate(origio);

    if (newio != NULL)
    {
        TARinput *newopaque = (TARinput *) xmalloc(sizeof (TARinput));
        newopaque->origio = newio;
        newopaque->fsize = input->fsize;
        newopaque->offset = input->offset;
        retval = (MojoInput *) xmalloc(sizeof (MojoInput));
        memcpy(retval, io, sizeof (MojoInput));
        retval->opaque = newopaque;
    } // if
#endif
    return retval;
} // MojoInput_tar_duplicate

static void MojoInput_tar_close(MojoInput *io)
{
    TARinput *input = (TARinput *) io->opaque;
    TARinfo *info = (TARinfo *) input->ar->opaque;
    //input->ar->io->close(input->ar->io);
    info->input = NULL;
    free(input);
    free(io);
} // MojoInput_tar_close


// MojoArchive implementation...

static boolean MojoArchive_tar_enumerate(MojoArchive *ar)
{
    TARinfo *info = (TARinfo *) ar->opaque;
    MojoArchive_resetEntry(&ar->prevEnum);
    if (info->input != NULL)
        fatal("BUG: tar entry still open on new enumeration");
    info->curFileStart = info->nextEnumPos = 0;
    return true;
} // MojoArchive_tar_enumerate


// These are byte offsets where fields start in the tar header blocks.
#define TAR_FNAME 0
#define TAR_FNAMELEN 100
#define TAR_MODE 100
#define TAR_MODELEN 8
#define TAR_UID 108
#define TAR_UIDLEN 8
#define TAR_GID 116
#define TAR_GIDLEN 8
#define TAR_SIZE 124
#define TAR_SIZELEN 12
#define TAR_MTIME 136
#define TAR_MTIMELEN 12
#define TAR_CHKSUM 148
#define TAR_CHKSUMLEN 8
#define TAR_TYPE 156
#define TAR_TYPELEN 1
#define TAR_LINKNAME 157
#define TAR_LINKNAMELEN 100
#define TAR_MAGIC 257
#define TAR_MAGICLEN 6
#define TAR_VERSION 263
#define TAR_VERSIONLEN 2
#define TAR_UNAME 265
#define TAR_UNAMELEN 32
#define TAR_GNAME 297
#define TAR_GNAMELEN 32
#define TAR_DEVMAJOR 329
#define TAR_DEVMAJORLEN 8
#define TAR_DEVMINOR  337
#define TAR_DEVMINORLEN 8
#define TAR_FNAMEPRE 345
#define TAR_FNAMEPRELEN 155

// tar entry types...
#define TAR_TYPE_FILE '0'
#define TAR_TYPE_HARDLINK '1'
#define TAR_TYPE_SYMLINK '2'
#define TAR_TYPE_CHARDEV '3'
#define TAR_TYPE_BLOCKDEV '4'
#define TAR_TYPE_DIRECTORY '5'
#define TAR_TYPE_FIFO '6'

static boolean is_ustar(const uint8 *block)
{
    return ( (memcmp(&block[TAR_MAGIC], "ustar ", TAR_MAGICLEN) == 0) ||
             (memcmp(&block[TAR_MAGIC], "ustar\0", TAR_MAGICLEN) == 0) );
} // is_ustar

static int64 octal_convert(const uint8 *str, const size_t len)
{
    int64 retval = 0;
    int64 multiplier = 1;
    const uint8 *end = str + len;
    const uint8 *ptr;

    ptr = str;
    while ((ptr != end) && (*ptr >= '0') && (*ptr <= '7'))
        ptr++;

    while (--ptr >= str)
    {
        uint64 val = *ptr - '0';
        retval += val * multiplier;
        multiplier *= 8;
    } // while

    return retval;
} // octal_convert


static const MojoArchiveEntry *MojoArchive_tar_enumNext(MojoArchive *ar)
{
    TARinfo *info = (TARinfo *) ar->opaque;
    boolean zeroes = true;
    boolean ustar = false;
    uint8 scratch[512];
    uint8 block[512];
    size_t fnamelen = 0;
    int type = 0;

    memset(scratch, '\0', sizeof (scratch));

    MojoArchive_resetEntry(&ar->prevEnum);
    if (info->input != NULL)
        fatal("BUG: tar entry still open on new enumeration");

    if (!ar->io->seek(ar->io, info->nextEnumPos))
        return NULL;

    // Find a non-zero block of data. Tarballs have two 512 blocks filled with
    //  null bytes at the end of the archive, but you can cat tarballs
    //  together, so you can't treat them as EOF indicators. Just skip them.
    while (zeroes)
    {
        if (ar->io->read(ar->io, block, sizeof (block)) != sizeof (block))
            return NULL;  // !!! FIXME: fatal() ?
        zeroes = (memcmp(block, scratch, sizeof (block)) == 0);
    } // while

    // !!! FIXME We should probably check the checksum.

    ustar = is_ustar(block);

    ar->prevEnum.perms = (uint16) octal_convert(&block[TAR_MODE], TAR_MODELEN);
    ar->prevEnum.filesize = octal_convert(&block[TAR_SIZE], TAR_SIZELEN);
    info->curFileStart = info->nextEnumPos + 512;
    info->nextEnumPos += 512 + ar->prevEnum.filesize;
    if (ar->prevEnum.filesize % 512)
        info->nextEnumPos += 512 - (ar->prevEnum.filesize % 512);

    // We count on (scratch) being zeroed out here!
    // prefix of filename is at the end for legacy compat.
    if (ustar)
        memcpy(scratch, &block[TAR_FNAMEPRE], TAR_FNAMEPRELEN);
    fnamelen = strlen((const char *) scratch);
    memcpy(&scratch[fnamelen], &block[TAR_FNAME], TAR_FNAMELEN);
    fnamelen += strlen((const char *) &scratch[fnamelen]);

    if (fnamelen == 0)
        return NULL;   // corrupt file.  !!! FIXME: fatal() ?

    ar->prevEnum.filename = xstrdup((const char *) scratch);

    type = block[TAR_TYPE];
    if (type == 0)  // some archivers do the file type as 0 instead of '0'.
        type = TAR_TYPE_FILE;

    if (ar->prevEnum.filename[fnamelen-1] == '/')
    {
        while (ar->prevEnum.filename[fnamelen-1] == '/')
            ar->prevEnum.filename[--fnamelen] = '\0';

        // legacy tar entries don't have a dir type, they just append a '/' to
        //  the filename...
        if ((!ustar) && (type == TAR_TYPE_FILE))
            type = TAR_TYPE_DIRECTORY;
    } // if

    ar->prevEnum.type = MOJOARCHIVE_ENTRY_UNKNOWN;
    if (type == TAR_TYPE_FILE)
        ar->prevEnum.type = MOJOARCHIVE_ENTRY_FILE;
    else if (type == TAR_TYPE_DIRECTORY)
        ar->prevEnum.type = MOJOARCHIVE_ENTRY_DIR;
    else if (type == TAR_TYPE_SYMLINK)
    {
        ar->prevEnum.type = MOJOARCHIVE_ENTRY_SYMLINK;
        memcpy(scratch, &block[TAR_LINKNAME], TAR_LINKNAMELEN);
        scratch[TAR_LINKNAMELEN] = '\0';  // just in case.
        ar->prevEnum.linkdest = xstrdup((const char *) scratch);
    } // else if

    return &ar->prevEnum;
} // MojoArchive_tar_enumNext


static MojoInput *MojoArchive_tar_openCurrentEntry(MojoArchive *ar)
{
    TARinfo *info = (TARinfo *) ar->opaque;
    MojoInput *io = NULL;
    TARinput *opaque = NULL;

    if (info->curFileStart == 0)
        return NULL;

    // Can't open multiple, since we would end up decompressing twice
    //  to enumerate the next file, so I imposed this limitation for now.
    if (info->input != NULL)
        fatal("BUG: tar entry double open");

    opaque = (TARinput *) xmalloc(sizeof (TARinput));
    opaque->ar = ar;
    opaque->fsize = ar->prevEnum.filesize;
    opaque->offset = info->curFileStart;

    io = (MojoInput *) xmalloc(sizeof (MojoInput));
    io->ready = MojoInput_tar_ready;
    io->read = MojoInput_tar_read;
    io->seek = MojoInput_tar_seek;
    io->tell = MojoInput_tar_tell;
    io->length = MojoInput_tar_length;
    io->duplicate = MojoInput_tar_duplicate;
    io->close = MojoInput_tar_close;
    io->opaque = opaque;
    info->input = io;
    return io;
} // MojoArchive_tar_openCurrentEntry


static void MojoArchive_tar_close(MojoArchive *ar)
{
    TARinfo *info = (TARinfo *) ar->opaque;
    MojoArchive_resetEntry(&ar->prevEnum);
    ar->io->close(ar->io);
    free(info);
    free(ar);
} // MojoArchive_tar_close


static void free_wrapper_noop(MojoInput *io) {}

MojoArchive *MojoArchive_createTAR(MojoInput *io)
{
    MojoArchive *ar = NULL;
    uint8 sig[512];
    int64 br = io->read(io, sig, 4);
    void (*free_wrapper)(MojoInput *io) = free_wrapper_noop;

    if ((!io->seek(io, 0)) || (br != 4))
        return NULL;

    if (br == 4)
    {
        // Look at the first piece of the file to decide if it is compressed,
        //  and if so, wrap the MojoInput in a decompressor.

        #if SUPPORT_TAR_GZ   // gzip compressed?
        if ((sig[0] == 0x1F) && (sig[1] == 0x8B) && (sig[2] == 0x08))
        {
            free_wrapper = free_gzip_input;
            io = make_gzip_input(io);
        } // if
        #endif

        // BZ2 compressed?
        #if SUPPORT_TAR_BZ2  // bzip2 compressed?
        if ((sig[0] == 0x42) && (sig[1] == 0x5A))
        {
            free_wrapper = free_bzip2_input;
            io = make_bzip2_input(io);
        } // if
        #endif
    } // if


    // Now see if this is a tar archive. We only support "USTAR" format,
    //  since it has a detectable header. GNU and BSD tar has been creating
    //  these for years, so it's okay to ignore other ones, I guess.
    br = io->read(io, sig, sizeof (sig));  // potentially compressed.
    if ((!io->seek(io, 0)) || (br != sizeof (sig)))
    {
        free_wrapper(io);
        return NULL;
    } // if

    if (!is_ustar(sig))
    {
        free_wrapper(io);
        return NULL;
    } // if

    // okay, it's a tarball, we're good to go.

    ar = (MojoArchive *) xmalloc(sizeof (MojoArchive));
    ar->opaque = (TARinfo *) xmalloc(sizeof (TARinfo));
    ar->enumerate = MojoArchive_tar_enumerate;
    ar->enumNext = MojoArchive_tar_enumNext;
    ar->openCurrentEntry = MojoArchive_tar_openCurrentEntry;
    ar->close = MojoArchive_tar_close;
    ar->io = io;
    return ar;
} // MojoArchive_createTAR

#endif // SUPPORT_TAR

// end of archive_tar.c ...

