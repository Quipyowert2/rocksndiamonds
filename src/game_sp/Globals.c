// ----------------------------------------------------------------------------
// Globals.c
// ----------------------------------------------------------------------------

#include "Globals.h"

static void ReadDemo();
void ReadLevel();

static char *VB_Name = "modGlobals";
// --- Option Explicit
// --- Option Compare Text
// --- Option Base 0

// --- const long StretchWidth = 16;
// --- const long StretchWidth2 = StretchWidth / 2;
// --- const long BaseWidth = 16;
// --- const int TwoPixels = 2;

boolean Original;
boolean Cracked;
boolean Level_Arg;
boolean EGA_Arg;
boolean Record_Fix;
boolean SpeedKeys;
boolean Level_Fix;
boolean Dead_Code;
boolean Redundant;
boolean Alignments;
boolean Ctrl_Alt_Fix;
boolean Protection;
boolean EP_ENHANCE;
boolean EP_DEMO;
boolean EP_DEBUG;
boolean EXTRASPEED;
boolean TIMINGFIX;
boolean SafeRecord;
boolean Norm_Time;
boolean EP_OLD8;
boolean SAVEGAME;
boolean HP_DEMO;
boolean ScreenFix;
boolean DemoRecordFix;
boolean DebugSwitch;
boolean Ver62;
boolean Ver62test;
boolean Ver63;
boolean Ver64;

int LevelNumber;
char *CurPath, *OrigPath, *TmpPath;
boolean LevelLoaded;
long SignatureDelay;

boolean bCapturePane;

int FieldWidth; // = 60
int FieldHeight; // = 24
int HeaderSize; // = 96
int FieldMax, LevelMax;
long FileMax;
int *PlayField16;
byte *PlayField8;
byte *DisPlayField;

// Public DisplayMin%, DisplayMax%, DisplayWidth%, DisplayHeight%

int TimerVar, RandomSeed;
currency DeltaT; // Interval between two frames (in ms)
long DeltaTPlay, DeltaTDemo;
boolean BlockingSpeed;

// --- const int posFrameCorner = 55;
// --- const int posFrameVertical = 110;
// --- const int posFrameHorizontal = 111;

int FreezeZonks;

// constants for  Fixed Fields:
// --- const int fiSpace = 0; // &H00  space(28 = wall space ...)
// --- const int fiZonk = 1; // &H01  zonk
// --- const int fiBase = 2; // &H02  base
// --- const int fiMurphy = 3; // &H03  Murphy
// --- const int fiInfotron = 4; // &H04  infotron
// --- const int fiRAM = 5; // &H05  small RAM chip
// --- const int fiHardWare = 6; // &H06  hardware (square, standard pyramid shape)
// --- const int fiExit = 7; // &H07  exit
// --- const int fiOrangeDisk = 8; // &H08  brown/orange utility disk
// --- const int fiPortRight = 9; // &H09  port 1 left to right
// --- const int fiPortDown = 10; // &H0A  port 1 up to down
// --- const int fiPortLeft = 11; // &H0B  port 1 right to left
// --- const int fiPortUp = 12; // &H0C  port 1 down to up
// --- const int fiSpPortRight = 13; // &H0D  port 2 left to right (gravity change)
// --- const int fiSpPortDown = 14; // &H0E  port 2 up to down     (gravity change)
// --- const int fiSpPortLeft = 15; // &H0F  port 2 right to left (gravity change)
// --- const int fiSpPortUp = 16; // &H10  port 2 down to up     (gravity change)
// --- const int fiSnikSnak = 17; // &H11  snik snak
// --- const int fiYellowDisk = 18; // &H12  yellow utility disk
// --- const int fiTerminal = 19; // &H13  terminal
// --- const int fiRedDisk = 20; // &H14  red utility disk
// --- const int fiPortUpAndDown = 21; // &H15  vertical port
// --- const int fiPortLeftAndRight = 22; // &H16  horizontal port
// --- const int fiPortAllDirections = 23; // &H17  horizontal + vertical port
// --- const int fiElectron = 24; // &H18  electron
// --- const int fiBug = 25; // &H19  bug
// --- const int fiRAMLeft = 26; // &H1A  horizontal RAM chip, left (pin 1)
// --- const int fiRAMRight = 27; // &H1B  horizontal RAM chip, right
// --- const int fiHWFirst = 28; // &H1C  hardware (radial blue circular cap + coloured shapes)

// Public Const fiHW1% = 29               '  29 = 1D  hardware (green signal lamp)
// Public Const fiHW2% = 30               '  30 = 1E  hardware (blue signal lamp)
// Public Const fiHW3% = 31               '  31 = 1F  hardware (red signal lamp)
// Public Const fiHW4% = 32               '  32 = 20  hardware (yellow/black diagonal stripes)
// Public Const fiHW5% = 33               '  33 = 21  hardware (yellow resistor + blue + red shapes)
// Public Const fiHW6% = 34               '  34 = 22  hardware (horizontal red capacitor + smd shape)
// Public Const fiHW7% = 35               '  35 = 23  hardware (red + yellow + blue horizontal resistors)
// Public Const fiHW8% = 36               '  36 = 24  hardware (3 red vertical resistors)
// --- const int fiHWLast = 37;             //  37 = 25  hardware (3 yellow horizontal resistors)
// --- const int fiRAMTop = 38;             //  38 = 26  vertical RAM chip, top (pin 1)
// --- const int fiRAMBottom = 39;          //  39 = 27  vertical RAM chip, bottom

// Specials to experiment with ...
// --- const int fiWallSpace = 40;          //  40 = 28  invisible wall (can explode, zonks don't roll off)
// --- const int fiHWTrash1 = 41;           //  41 = 29  hardware trash
// --- const int fiHWTrash2 = 42;           //  42 = 2A  hardware trash
// --- const int fiHWMurphy = 43;           //  43 = 2B  hardware inverted Murphy ... (maybe nice for use?)

// --- const int fiExplosion = 0x1F;

// --- const int keyNone = 0;
// --- const int keyUp = 1;
// --- const int keyLeft = 2;
// --- const int keyDown = 3;
// --- const int keyRight = 4;
// --- const int keySpaceUp = 5;
// --- const int keySpaceLeft = 6;
// --- const int keySpaceDown = 7;
// --- const int keySpaceRight = 8;
// --- const int keySpace = 9;

int *aniBug, *aniZonkRollRight, *aniZonkRollLeft;
int *aniInfotronRollRight, *aniInfotronRollLeft;
int *aniSnikSnak, *aniElectron, *aniExplosion;
int *aniTouchBase, *aniTouchInfotron, *aniTouchRedDisk;
// --- const int aniExplosionInfo = 111;
// --- const int aniSnikSnakUp = 159;
// --- const int aniSnikSnakDown = 167;
// --- const int aniSnikSnakLeft = 239;
// --- const int aniSnikSnakRight = 247;
// --- const int aniMurphyYawn = 56;
// --- const int aniMurphySleepLeft = 71;
// --- const int aniMurphySleepRight = 68;
int *aniMurphyExit; // , aniMurphyFaceLeft%, aniMurphyFaceRight%
int *aniMurphyEatLeft, *aniMurphyEatRight; // , aniMurphyEatRightRedDisk
int *aniMurphyEatUpLeft, *aniMurphyEatUpRight, *aniSplitUpDown;
int *aniYellowDisk, *aniOrangeDisk, *aniRedDisk;
// --- const int aniMurphyTouchUp = 46;
// --- const int aniMurphyTouchLeft = 95;
// --- const int aniMurphyTouchDown = 47;
// --- const int aniMurphyTouchRight = 94;
int *aniEatInfotronLeft, *aniEatInfotronRight;
// --- const int aniPushLeft = 45;
// --- const int aniPushRight = 44;
// --- const int aniPushUpDown = 79;

void InitGlobals()
{
  aniBug = Array(74, 75, 76, 77, 78, 77, 76, 77, 78, 77, 76, 75, 74, 25);
  aniZonkRollRight = Array(198, 197, 196, 195, 194, 193, 192, 1, -1);
  aniZonkRollLeft = Array(192, 193, 194, 195, 196, 197, 198, 1, -1);
  aniInfotronRollRight = Array(206, 205, 204, 203, 202, 201, 200, 4);
  aniInfotronRollLeft = Array(200, 201, 202, 203, 204, 205, 206, 4);
  aniSnikSnak = Array(121, 122, 123, 124, 125, 126, 127, 120, 121);
  aniElectron = Array(144, 145, 146, 147, 148, 149, 150, 151, 144);
  aniExplosion = Array(3, 103, 104, 105, 106, 107, 108, 109, 0);
  aniTouchBase = Array(80, 81, 82, 83, 84, 85, 86, 0, -1);
  aniTouchInfotron = Array(87, 88, 89, 91, 92, 93, 0, -1); // Only seven frames!!!!
  aniTouchRedDisk = Array(96, 97, 98, 99, 100, 101, 102, 0, -1);
  aniMurphyExit = Array(46, 46, 46, 46, 47, 47, 47, 47, 48, 48, 48, 48, 49, 49, 49, 49, 50, 50, 50, 50, 51, 51, 51, 51, 52, 52, 52, 52, 53, 53, 53, 53, 54, 54, 54, 54, 0, 0, 0, 0, -1);
  aniMurphyEatLeft = Array(176, 177, 178, 179, 180, 181, 182, 183, -1);
  aniMurphyEatRight = Array(184, 185, 186, 187, 188, 189, 190, 191, -1);
  aniMurphyEatUpLeft = Array(183, 182, 181, 180, 179, 178, 177, 176, -1);
  aniMurphyEatUpRight = Array(191, 190, 189, 188, 187, 186, 185, 184, -1);
  // aniMurphyEatRightRedDisk = Array(184, 184, 185, 186, 187, 188, 189, 190, 191, -1) '9 frames!
  aniEatInfotronLeft = Array(209, 211, 213, 215, 217, 219, 221, 223, -1);
  aniEatInfotronRight = Array(224, 226, 228, 230, 232, 234, 236, 238, -1);
  aniSplitUpDown = Array(3, 3, 3, 3, 3, 3, 3, 3, -1);
  aniYellowDisk = Array(18, 18, 18, 18, 18, 18, 18, 18, -1);
  aniOrangeDisk = Array(8, 8, 8, 8, 8, 8, 8, 8, -1);
  aniRedDisk = Array(20, -1);
  InitPseudoCompileFlags();
  UserDragFlag = False;
  AutoScrollFlag = True;
  FreezeZonks = 0;
  BlockingSpeed = False;
  LevelLoaded = False;
  FieldWidth = 60;
  FieldHeight = 24;
  HeaderSize = 96;
  FieldMax = (FieldWidth * FieldHeight) + HeaderSize - 1;
  LevelMax = (FieldWidth * FieldHeight) - 1;
  Let_ModifiedFlag(False);
  bPlaying = False;
  gSignature = "";
  bSignatureAvailable = False;
  FirstDemoByte = 0x81;
  MySignature = "";
  InitErrorReporting();
}

void InitPseudoCompileFlags()
{
  Cracked = 1;       // If protection Then crack it
  Level_Arg = 1;     // :number is cmd line option
  // Level_Arg  = 0     ' Remove Level cmd line option
  EGA_Arg = 1;       // EGA is command line option
  Record_Fix = 1;      // Assemble with fixed Demo rec
  SpeedKeys = 0;     // Remove Speed Keys fix
  Level_Fix = 1;     // Assemble with Level Fix
  Dead_Code = 0;     // Remove dead code
  Redundant = 0;     // Remove redundant code
  Alignments = 1;      // Assemble with alignments
  Ctrl_Alt_Fix = 1;      // Assemble with Ctrl/Alt fix
  Protection = 0;      // Remove protection code,do HP
  // EP added by EP for version 5.
  EP_ENHANCE = 1;      // Some more nice things (EP)
  EP_DEMO = 1;       // Use .SP files for demos (EP)
  // Including record demo!
  EP_DEBUG = 0;      // little cmdline debugging
  EXTRASPEED = 1;      // '@' option, superfast!
  TIMINGFIX = 1;     // "Fixed" the timing problem..
  // Inactive If DemoRecordFix Then1
  SafeRecord = 1;      // skip debug keys in recording
  Norm_Time = 1;     // force automatic speed test,
  // save result and then do as
  // requested from cmd line.
  EP_OLD8 = 1;       // call old int8 from current.
  SAVEGAME = 1;      // Allow saving to SUPAPLEX.SAV
  HP_DEMO = 1;       // Use fixed demo routines 5.5
  ScreenFix = 1;     // No menu-write to gamy field
  DemoRecordFix = 1;     // Demo record timing fix on
  DebugSwitch = 1;     // Allow Ctrl/Alt-ScrollLock
  Ver62 = 1;       // Version 6.2 stuff
  Ver62test = 0;     // Version 6.2 test stuff
  Ver63 = 1;       // Version 6.3 stuff
  Ver64 = 1;       // Version 6.4 stuff

}

int GetSI(int X, int Y)
{
  int GetSI;

  GetSI = Y * FieldWidth + X;

  return GetSI;
}

int GetX(int si)
{
  int GetX;

  GetX = si % FieldWidth;

  return GetX;
}

int GetY(int si)
{
  int GetY;

  GetY = si / FieldWidth;

  return GetY;
}

int GetStretchX(int si)
{
  int GetStretchX;

  GetStretchX = StretchWidth * (si % FieldWidth);

  return GetStretchX;
}

int GetStretchY(int si)
{
  int GetStretchY;

  GetStretchY = StretchWidth * (si / FieldWidth);

  return GetStretchY;
}

void ReadLevel()
{
  // int FNum;
  FILE *FNum;
  long i;
  // byte T;

  DemoAvailable = False;
  if (STRING_IS_LIKE(CurPath, "*.mpx"))
  {
    ReadMPX();
    return;
  }

  if (STRING_IS_LIKE(CurPath, "*.sp"))
  {
    ReadDemo();
    return;
  }

  if (DemoFlag != 0)
  {
    ReadDemo();
    return;
  }

  FileMax = 0;
  FieldWidth = 60;
  FieldHeight = 24;
  HeaderSize = 96;
  FieldMax = (FieldWidth * FieldHeight) + HeaderSize - 1;
  LevelMax = (FieldWidth * FieldHeight) - 1;
  PlayField8 = REDIM_1D(sizeof(byte), 0, FieldMax + 1 - 1);
  DisPlayField = REDIM_1D(sizeof(byte), 0, FieldMax + 1 - 1);
  // FNum = FreeFile();

  // --- On Error GoTo ReadLevelEH
  FNum = fopen(CurPath, "rb");
  i = (LevelNumber - 1) * ((long)(FieldMax) + 1) + 1;
  FILE_GET(FNum, i, &PlayField8, sizeof(PlayField8));
  i = (LevelNumber) * ((long)(FieldMax) + 1) + 1 - HeaderSize;
  FILE_GET(FNum, i, &LInfo, sizeof(LInfo)); // store level info in an extra structure
  fclose(FNum);
  // --- On Error GoTo 0

  if (FieldMax < FileMax)
    DemoAvailable = True;

  ReadSignature();
  PlayField16 = REDIM_1D(sizeof(int), -FieldWidth, FieldMax);
  for (i = 0; i <= FieldMax; i++)
  {
    PlayField16[i] = PlayField8[i];
    DisPlayField[i] = PlayField8[i];
    PlayField8[i] = 0;
  }

  AnimationPosTable = REDIM_1D(sizeof(int), 0, LevelMax - 2 *FieldWidth);
  AnimationSubTable = REDIM_1D(sizeof(byte), 0, LevelMax - 2 *FieldWidth);
  TerminalState = REDIM_1D(sizeof(byte), FieldWidth, LevelMax - FieldWidth);
  GravityFlag = LInfo.InitialGravity;
  FreezeZonks = LInfo.InitialFreezeZonks;
  subRandomize();
  LevelLoaded = True;
  return;

#if 0
ReadLevelEH:
  Close();
#endif
}

static void ReadDemo()
{
  // int FNum, i;
  FILE *FNum;
  int i;
  // byte T;

  FieldWidth = 60;
  FieldHeight = 24;
  HeaderSize = 96;
  FieldMax = (FieldWidth * FieldHeight) + HeaderSize - 1;
  LevelMax = (FieldWidth * FieldHeight) - 1;

  // --- On Error GoTo ReadDemoEH
  FileMax = FileLen(CurPath) - 1;
  PlayField8 = REDIM_1D(sizeof(byte), 0, FileMax + 1 - 1);
  DisPlayField = REDIM_1D(sizeof(byte), 0, FieldMax + 1 - 1);
  // FNum = FreeFile();
  FNum = fopen(CurPath, "rb");
  i = (LevelNumber - 1) * ((long)(FieldMax) + 1) + 1;
  FILE_GET(FNum, i, &PlayField8, sizeof(PlayField8));
  i = (LevelNumber) * ((long)(FieldMax) + 1) + 1 - HeaderSize;
  FILE_GET(FNum, i, &LInfo, sizeof(LInfo)); // store level info in an extra structure
  fclose(FNum);
  // --- On Error GoTo 0

  if (FieldMax < FileMax)
    DemoAvailable = True;

  ReadSignature();
  PlayField16 = REDIM_1D(sizeof(int), -FieldWidth, FieldMax);
  for (i = 0; i <= FieldMax; i++)
  {
    PlayField16[i] = PlayField8[i];
    DisPlayField[i] = PlayField8[i];
    PlayField8[i] = 0;
  }

  AnimationPosTable = REDIM_1D(sizeof(int), 0, LevelMax - 2 *FieldWidth);
  AnimationSubTable = REDIM_1D(sizeof(byte), 0, LevelMax - 2 *FieldWidth);
  TerminalState = REDIM_1D(sizeof(byte), 0, FieldMax + 1 - 1);
  DemoPointer = FieldMax + 1;
  DemoOffset = DemoPointer;
  DemoKeyRepeatCounter = 0;
  // DemoFlag = 1
  // DemoAvailable = True
  GravityFlag = LInfo.InitialGravity;
  FreezeZonks = LInfo.InitialFreezeZonks;
  RandomSeed = LInfo.DemoRandomSeed;
  LevelLoaded = True;
  return;

#if 0
ReadDemoEH:
  Close();
#endif
}