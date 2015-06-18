
This is the second patch for Unreal Tournament 3. Please provide any
and all feedback on this patch at:


    http://utforums.epicgames.com/forumdisplay.php?f=21.


Gameplay:

- Increased UTGame MaxPlayersAllowed to 64.
- Fixed leviathan turret instant refire exploit.
- Fixed errant lock on warnings when no longer in vehicle.
- Fixed first person weapons in demo playback.
- Fixed translocator telefrag victim message.
- Fixed encouragement sounds not being randomly picked by bots.
- Implemented viewobjective spectating system for Warfare.
- Fixed berserk held by driver applying to all vehicle turrets.
- Only force low gore on German versions that were low gore
  only before being patched.
- Fix for unnecessary content staying in memory on seamless travel.
- Fixed shaped charge node exploit.
- Fixed Scavenger exploit.


User Interface:

- Clicking on the settings tab goes to directly to the full
  settings menu.
- Added VOIP speaker portraits to HUD.
- Fixed character portraits sometimes not showing up on HUD or
  not staying up long enough.
- The Host server menus will no longer disable internet options if
  CheckNatTypeDisplayError returns true.  It's just a warning now.
- Fixed the CD key always prompting when the user has no network card
- Added support for commandline log in -login=xxx -password=xxx.
  This also allows Gamespy comrade and other external applications to
  be used to launch network clients.
- Added UI option to hide objective paths (the white arrows).
- Added UI option to enable joystick support.
- Added JOIN to midgame menu when spectating.
- Added cancel button to "logging in" message box.
- Added support for auto-updating UI with new options.
- Improved language support for French, Spanish, Italian, and German.
- Joystick key bindings in UI display properly.
- Added game and UI support for customizing crosshair scaling.
- Added "Add Favorite" button to Server Browser server list tab.
- Fixed favorites Tab Page server details not updating.
- Improvements to voice menu. Added "status" section.
- Leave "Disconnect" and "Exit Game" on the mid game menu when
  seamlessly travelling so that players have a way to abort lengthy
  downloads
- Show "Change Team" button before the match has started.
- Added support for localized "single score needed" string.
- Tweaked some HUD message font sizes.
- Favorites/History lists list servers that are currently offline.
- Favorites/History lists don't stop working as the number of
  servers in them crosses max threshold for GS query.
- Fix for end of round scoreboard displaying extra "Player" bots.
- Fixed leviathan deploy icon positioning.


Networking:
- Added support for autodownloading packages while in gameplay
  or while travelling.
- Auto team re-balancing before map transition if
  bPlayersBalanceTeams is set.
- Fixed dedicated server memory leaks.
- Force client state synchronization when in spectating state.
  Fixes sporadic issues with players not being able to join games.
- Fixed sounds not being heard correctly by clients if the
  sound location is the Actor's location and the Actor is not relevant
  to that client.
- Fixed spectators being unable to move after a level transition.
- Fixed track turrets being in the wrong position on clients in some
  cases after being destroyed.
- ConnectionTimeout and InitialConnectTimeOut now both 60.0.
  Addresses both clients failing connection because they take too
  long to load a level, and initial connections staying open too long.
- Fixed HTTP download compression.
- Fixed a bug where download during seamless travel would break because
  the downloader was keeping a pointer into the package map's list,
  which is unsafe while the game is in progress because that list could
  have more items added to it (and thus the memory rearranged) at any time.
- Fixed a bug where in rare cases packages would get downloaded twice.
- Fixed spectators able to enter as extra players in Duel.
- Fixed issue where CTF, vCTF, and WAR maps are listed in the server
  browers when searching for a DM game when server changed gametypes.
- Fixed bots not replicating their view pitch, so their animation looks
  better in net games.
- Strip OwningPlayerName= from the URL, as this is determined elsewhere
  and was breaking dedicated internet servers launched from the UI.
- Fixed package downloading not moving on to the next download method
  if it successfully downloads the file, but that file is not the version
  that the server is requesting.
- Fixed client crash if the user disconnects while downloading a file
  during a seamless travel.

 
Server administration:

- TCPLink and Webadmin functionality implemented.
- Banning is now based on CD key hash, so players can't circumvent bans
  by creating a new profile.
- Fixed being unable to kickban players with | in their names.
- Added versioning information to the game settings
- Dedicated servers don't require DirectX shader model 2.0
- Fixed AdminForceTextMute and AdminForceTextUnmute.
- Restored the compress and decompress commandlets.
- Fixed AdminCmdOk() function not working properly if you were
  the listen server.
- Banned IDs readability improved.
- Added MaxClientTravelTime config option to GameInfo. If set,
  clients are kicked if they take longer than this many seconds to
  travel between maps.

 
Mod support:

- Added ScriptedTexture, a type of render to texture that gives Canvas
  access to script/C++ for rendering custom overlays.
- Added a version of DrawTile() to Canvas that takes a Texture
  instead of Texture2D so render to texture stuff like ScriptedTextures
  can more easily be used.
- Added Timestamp function to UObject, returns a string in the
  format YYYY/MM/DD - HH:MM:SS
- Weapons now take roll from player viewrotation.
- Fixed custom character DLC not applying until the next time
  the game is run because the default object was not updated after
  combining the .ini files.
- Added a ModFamilies array to UTCustomChar_Data to allow mod
  authors to add families to the list.
- Fixed Change Node Status Kismet action not working on fully
  constructed powernodes.
- Merged Ageia particle fixes.

 

Map specific:

- Fixed DM-Deck get out of world exploit
- Fixed DM-Gateway portals sometimes sending you back to your
  starting point.
- Fixed CTF-Coret collision exploit.
- Fixed kismet spawned key vehicles not showing on host
  minimap.  Fixes tank showing on host minimap in VCTF-Kargo.

 

AI improvements:

- New orb carrier strategy AI.
- Improved bot hoverboard use.
- Reduced bot orb spawner camping.
- Fixed bots stuck on orb spawner unable to grab flag.
- Low skill bots use artillery properly.
- Tweaked shooting at nodes vs shooting at enemies.
- No human bonus to threat value.
- Improved threat picking AI, taking into account effectiveness
  of bot's weapon.
- Tweaked bot AI for link gun, flak cannon, redeemer, and AVRiL.
- Bots tend to stay on same enemy more, and focus on key vehicles more.
- Tweaked campaign auto skill adjust.
- Possible safe fix for bot navigation issues with staticmeshcollections
  on console. Also a performance improvement on PC and console.
- Tweaked rules for whether to attack node or enemy first.
- Improved bot AI for defending nodes with an orb.
- Fixed pathing issues that were causing bots to get stuck in
  some places.  When a move fails, force a route refresh.  Add cost to
  reachspecs when bot fails repeatedly.
- Improved AI code for adjusting around obstacles.  If adjust
  left and right fail, try moving to center of reachspec.
- If previous move failed, don't allow "advanced tactics"
  (serpentine, etc.) for next move.
- Added FailedMoveTarget and MoveFailureCount to controller to
  track movement failures.
- Added bForceNoDetours to UTBot.  Don't allow detours when
  bot is approaching a neutral node (more important to touch node first).
- Fixed bots thinking they've reach the orb spawner without
  quite getting there and touching the orb.
- Fixed cases where bots could get stuck failing to
  translocate over and over - bots know to give up now.
- Adjusted bot reaction time to seeing new enemies.
- Improved bot AI for dealing with lifts and hoverboards.
- Bots taunt after winning a match.
- Bots now can be fooled by feign death.
- Sandstorm has more impact on bots being able to acquire/aim at enemies.

 

-----------------------------------------------------------------------



Changes from the first patch are also included.  These changes are
listed below:

 

Gameplay:

- Fixed scaling of certain player meshes (increased in size
  some human and robot meshes).  Addresses meshes being smaller than
  collision box, as well as eyeheight issues.
- Fixed feigning death into/through ForcedDirVolumes.
- Fixed grenade effects in water.
- Increased hellbender rear turret damage.
- Reduced Goliath machine gun spread, plus slight damage increase.
- Fixed warfare scoring for locking down a prime node not
  called "prime node".
- Slightly increased momentum taken from damage by mantas and vipers.
- Flak, Rocket, and Shock do slightly more damage to manta and viper.
- Made sure Hellfire SPMA cannon can't fire through walls.
- Increased incoming SPMA fire sound radius.
- Fixed impact jumping with hoverboard.
- Fixed bot Pawns losing their PRI at the end of the match,
  causing them to, among other things, be invisible.
- Fixed being able to switch away from the rocket launcher in
  the delay between the third rocket being loaded and the weapon
  autofiring.
- Players now stop moving when they fire the Redeemer guided warhead.
- Fixed weapon crosshair incorrectly turning red when hit enemy on the
  client, but not on the server.
- Disabled attenuation/spatialization on mission briefing sounds.
- Fixed stats being recorded for spectators


AI:

- Improved bot AI with darkwalker.
- Tweaked bot voice message frequencies.
- Bot aiming tweaks.
- Fixed bots attacking friendly player in rare cases when that player
  recently stole an enemy vehicle.
- Fixed bots not handling the "Hold This Position" order correctly
  when the player giving the order is in a multi-person vehicle.


Demo playback:

- Fixed demo playback not ending/looping correctly when the demo ends
  due to the DemoRecSpectator being destroyed before the end of
  the file is reached.
- Demos can now be paused.
- By default, demo playback now runs at full speed and interpolates
  in between demo frames using the normal client simulation code. The
  old frame-locked method is still available by passing ?disallowinterp.
  Timedemos are unaffected by this change.
- Fixed demoplay URL parsing counting the options as part of
  the filename unless an extension was specified in the demo name
- Added a "Delete Demo" button to the demo playback menu.
- Demo playback now properly handles rotation when viewing a
  Pawn in first person.
- Fixed looking around while spectating a vehicle.

 
Server Browser:

- Implemented "Server History" tab page in server browser, with ability
  to "lock" favorites on that page.
- Added 'Join as spectator' feature.
- Server browser uses smaller font to display more servers.
- Fixed custom mutators not appearing in server browser.
- Fixed custom gametypes not displayed in server browser's window.
- Fixed server browser's listed MaxPlayers being incorrect.
- Added filter option for dedicated servers.
- Fixed server browser showing an incorrect goal score and time limit
  when the .ini values were used.
- Fixed incorrect mutators appearing in server browser details if client
  and server are not using the same language.


User Interface:

- Can now save settings/progress even if have never created a profile.
- Added ping and connect time to scoreboard.
- Removed annoying confirmation menu when starting a game.
- Removed unnecessary top settings page (can use tabs at the
  top of the settings to navigate).
- Added a Messages tab to the mid game menu.
- Friends messages now saved until explicitly deleted.
- Finer control over mouse sensitivity, using an edit box
  instead of a slider.
- Added framerate smoothing and FOV options to the advanced video menu.
- Increased max players/bots in menus to 32.
- Fixed auto switching to vote menu at end of match.
- Improved mid game menu performance (don't render world behind it).
- Added version number to main menu.
- Fixed binding gamepad/joystick keys (you must set AllowJoystickInput=1
  in the [WinDrv.WindowsClient] of your UTEngine.ini to enable
  gamepads/joysticks).
- Filter settings are now saved to the .ini file
- Pure and Locked filter options now default to "Any"
- Fixed flickering when downloading files
- Fixed up the GDF project for Vista

 
HUD:

- Added the killer weapon to victim messages.
- Fixed flag and orb scaling in minimap at high resolutions.
- Fixed node teleporter not showing "You can't teleport with orb" message
  on clients.
- Fixed situations where "get in vehicle" pictograph wouldn't
  work correctly.
- Still draw the clock on the scoreboard after the game is over.
- Fixed Duel HUD issues.
- Added the ability to specify custom simple crosshair coordinates
  for weapons, with new config properties bUseCustomCoordinates and
  CustomCrosshairCoordinates.

    To change the crosshair used for all weapons, add the following
    lines to the [utweapon] section of the UTWeapon.ini file:

       bUseCustomCoordinates=true
       CustomCrosshairCoordinates=(U=276,V=84,UL=22,VL=25)

    where the CustomCrosshairCoordinates have U and V as the offsets
    into the crosshair texture (UI_HUD.HUD.UTCrosshairs), and UL and VL
    specify the size of the texture area to use.
    To use a different crosshair for a specific weapon, simply add
    those two lines to the appropriate weapon specific section in
    UTWeapon.ini.

 

Networking:

- Implemented STUN support (Simple Traversal of UDP Through
Network Address Translators) to enable clients and servers to connect
from behind a NAT.
- Fixed team scores very rarely not updating for a client.
- Fixed bot faction option when running a listen server.
- Fixed link setup not reset correctly when going from a map
  with a custom link setup to one using the default.
- Fixed a case where Duel would place an incoming player on
  the wrong team when some players were still travelling.
- Bullseye stats are now properly recorded.
- Fixed clients not travelling to downloaded maps correctly.
- Fixed the client and server getting into a loop sending each
  other close messages in some situations.
- Quick match incorporates player rating into search decision.
- Fixed issue where Vista clients would not receive all
  servers from a server browser search.
- Applied proper fix to suppressing voice on dedicated servers.
- Fixed issues with players not getting on right team in Duel
  and Duel+Survival if a player leaves in certain timing windows
  during map transitions.
- Fixed previous level PRIs showing up on the scoreboard/leaderboard
  after the client travels (making it look like players have already
  scored lots of points when those clients are in fact still loading)
- Fixed losing your custom character mesh after changing teams
  during a match.
- Fixed HTTP redirection for mod autodownloading
- Fixed autodownloaded mods not being loaded correctly in some cases.
- Fixed orb rebuilding not being played correctly on clients
  if the orb was destroyed by an enemy player.
- Implemented DUEL match stat reporting for gamespy ladder.
- Fixed "open" console command not working when an Internet
  server address is specified.
- Added "BecomeActive" exec to switch from spectator to player.


Server Administration:

- Reduced tick rate for dedicated servers with no clients
  (saves CPU on idle servers).
- Dedicated servers do not require CD keys.
- Added -configsubdir= command line option to cause .ini files
  to be loaded/saved from the specified subdirectory of Game\Config\
- Added QueryPort configuration and command line option.
- Added an "AdminChangeOption [option] [value]" console
  command for server admins. This allows changing most simple .ini
  values (e.g. GoalScore) from the client. This command will not
  override URL options. The change will take effect after the next map
  change.
- Added an "AdminPublishMapList" console command for server
  admins. This overrides the server's map list for the current game type
  with the one on the client that used the command.
- Uses GameReplicationInfo.ServerName if set for the name of
  the server on the server browser.
- Added "AdminForceVoiceMute" and "AdminForceVoiceUnMute":
  Stops/Starts a player from sending voip to others
- Added "AdminForceTextMut" and "AdminForceTextUnMute":
  Stops/Starts a player from send text messages to others
- Updated AdminPlayerList to show the PlayerID of the players
  on the server.
- Updated Kick/Ban to allow for using either the player name or the id.
- Fixed servers advertising as the wrong gametype if the
  gametype is changed without restarting the server
- Added new IdleServerTickRate property to IpDrv.TcpNetDriver.
  If not set, IdleServerTickRate defaults to MaxTickRate. Can be set to
  lower values to reduce server CPU utilization when 0 players. Reported
  ping will increase if set lower.

 

Map Specific:

- Fixed WAR-Avalanche terrain LOD popping issue on high end PCs.
- Fixed bots rarely getting stuck in mid air in DM-Gateway in
  the city section.
- Fixed some VCTF-Suspense pathing issues.
- Fixed issues with circular lift on DM-Deimos.
- Fixed translucent mesh sorting issues in DM-Gateway.
- Fixed various map collision bugs
- Improved bot AI with Leviathan in Torlan

 

Campaign/Co-op:

- Fixed a bug that could cause too many bots to be added to
  co-op matches in some cases.
- Added support for seamless travel interrupting a travel to
  start another travel. This fixes single player breaking if the host
  selects the next mission before the clients have finished travelling to
  the mission selection level
- Fixed extra copy of a character when a human player leaves a
  co-op game at the right time.
- Changed network loss during single player to result in
  player signed in locally.

 

Modding:

- Added a SupportedGameTypes field to UTUIDataProvider_Mutator.
  If some entries are in this array, the mutator will only be visible
  in the menus if the selected gametype is found in the array.
- Fixed custom gametype midgame menus not being used correctly.
- Shipping script compiler now allows localized/config
  defaultproperties because otherwise autodownloaded mods have no
  way for their localized/config variables to work.
- Added Get/SetSpecialValue() stubs to Object to allow mods to
  expose values that can be modified without creating a dependency.
- Added support for custom gametypes showing up on their own
  in the server browser. The game class needs to implement
  UpdateGameSettings() and call GameSettings.SetStringSettingValue() to
  set CONTEXT_GAME_MODE to CONTEXT_GAME_MODE_CUSTOM. Then, in the
  gametype's menu .ini data, set GameSearchClass to "UTGameSearchCustom".
- Fixed cooking a map sometimes deleting all other mod files
  in the Published directory.
- Fixed bPostRenderIfNotVisible flag.


// end of README ...

