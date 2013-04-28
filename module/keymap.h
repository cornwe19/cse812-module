#ifndef __KEYMAP__
#define __KEYMAP__

#define MULTIMEDIA 0

static char* SHIFT_KEYMAP[] = {
    [0x01] = "[Esc]",
    [0x02] = "!",
    [0x03] = "@",
    [0x04] = "#",
    [0x05] = "$",
    [0x06] = "%",
    [0x07] = "^",
    [0x08] = "&",
    [0x09] = "*",
    [0x0A] = "(",
    [0x0B] = ")",
    [0x0C] = "_",
    [0x0D] = "+",
    [0x0E] = "[Bksp]",
    [0x0F] = "[Tab]",
    [0x10] = "Q",
    [0x11] = "W",
    [0x12] = "E",
    [0x13] = "R",
    [0x14] = "T",
    [0x15] = "Y",
    [0x16] = "U",
    [0x17] = "I",
    [0x18] = "O",
    [0x19] = "P",
    [0x1A] = "{",
    [0x1B] = "}",
    [0x1C] = "[Enter]",
    [0x1D] = "[Ctrl]",
    [0x1E] = "A",
    [0x1F] = "S",
    [0x20] = "D",
    [0x21] = "F",
    [0x22] = "G",
    [0x23] = "H",
    [0x24] = "J",
    [0x25] = "K",
    [0x26] = "L",
    [0x27] = ":",
    [0x28] = "\"",
    [0x29] = "~",
    [0x2A] = "[Shift]",
    [0x2B] = "|",
    [0x2B] = "[INT 2]",
    [0x2C] = "Z",
    [0x2D] = "X",
    [0x2E] = "C",
    [0x2F] = "V",
    [0x30] = "B",
    [0x31] = "N",
    [0x32] = "M",
    [0x33] = "<",
    [0x34] = ">",
    [0x35] = "?",
    [0x36] = "[Shift]",
    [0x37] = "[PrtSc]",
    [0x54] = "[PrtSc]",
    [0x38] = "[Alt]",
    [0x39] = " ",
    [0x3A] = "[CapsLock]",
    [0x3B] = "[F1]",
    [0x3C] = "[F2]",
    [0x3D] = "[F3]",
    [0x3E] = "[F4]",
    [0x3F] = "[F5]",
    [0x40] = "[F6]",
    [0x41] = "[F7]",
    [0x42] = "[F8]",
    [0x43] = "[F9]",
    [0x44] = "[F10]",
    [0x45] = "[Num Lock]",
    [0x46] = "[Scroll Lock]",
    [0x47] = "[Home]",
    [0x48] = "[Up]",
    [0x49] = "[PgUp]",
    [0x4A] = "-",
    [0x4B] = "[Left]",
    [0x4C] = "[KP]",
    [0x4D] = "[Right]",
    [0x4E] = "+",
    [0x4F] = "[End]",
    [0x50] = "[Down]",
    [0x51] = "[PgDn]",
    [0x52] = "[Ins]",
    [0x53] = "[Del]",
    [0x54] = "[SysRq]",
    [0x56] = "[INT 1]",
    [0x57] = "[F11]",
    [0x58] = "[F12]",
    [0x5B] = "[Win L]",
    [0x5C] = "[Win R]",
    [0x5D] = "[WinMenu]",
    [0x69] = "[Left]",
    [0x6A] = "[Right]",
    [0x67] = "[Up]",
    [0x6C] = "[Down]",
    [0x70] = "[katakana]",
    [0x73] = "[INT 3]",
    [0x77] = "[furigana]",
    [0x79] = "[kanji]",
    [0x7B] = "[hiragana]",
    [0x7D] = "[INT 4]",
    [0x7E] = "[INT 5]",
#if MULTIMEDIA
    [0x5E] = "[Power]",
    [0x5F] = "[Sleep]",
    [0x63] = "[Wake]",
    [0x20] = "[Mute]",
    [0x30] = "[Volume Up]",
    [0x2E] = "[Volume Down]",
    [0x17] = "[Cut]",
    [0x18] = "[Copy]",
    [0x0A] = "[Paste]",
    [0x3B] = "[Help]",
    [0x08] = "[Undo]",
    [0x07] = "[Redo]",
    [0x22] = "[Play]",
    [0x24] = "[Stop]",
    [0x10] = "[Skip Back]",
    [0x19] = "[Skip Fwd]",
    [0x2C] = "[Eject]",
    [0x1E] = "[Mail]",
    [0x32] = "[Web]",
    [0x3C] = "[Music]",
    [0x64] = "[Pictures]",
    [0x6D] = "[Video]"
#endif
};

static char* KEYMAP[] = {
    [0x01] = "[Esc]",
    [0x02] = "1",
    [0x03] = "2",
    [0x04] = "3",
    [0x05] = "4",
    [0x06] = "5",
    [0x07] = "6",
    [0x08] = "7",
    [0x09] = "8",
    [0x0A] = "9",
    [0x0B] = "0",
    [0x0C] = "-",
    [0x0D] = "=",
    [0x0E] = "[Bksp]",
    [0x0F] = "[Tab]",
    [0x10] = "q",
    [0x11] = "w",
    [0x12] = "e",
    [0x13] = "r",
    [0x14] = "t",
    [0x15] = "y",
    [0x16] = "u",
    [0x17] = "i",
    [0x18] = "o",
    [0x19] = "p",
    [0x1A] = "[",
    [0x1B] = "]",
    [0x1C] = "[Enter]",
    [0x1D] = "[Ctrl]",
    [0x1E] = "a",
    [0x1F] = "s",
    [0x20] = "d",
    [0x21] = "f",
    [0x22] = "g",
    [0x23] = "h",
    [0x24] = "j",
    [0x25] = "k",
    [0x26] = "l",
    [0x27] = ";",
    [0x28] = "'",
    [0x29] = "`",
    [0x2A] = "[Shift]",
    [0x2B] = "\\",
    [0x2B] = "[INT 2]",
    [0x2C] = "z",
    [0x2D] = "x",
    [0x2E] = "c",
    [0x2F] = "v",
    [0x30] = "b",
    [0x31] = "n",
    [0x32] = "m",
    [0x33] = ",",
    [0x34] = ".",
    [0x35] = "/",
    [0x36] = "[Shift]",
    [0x37] = "[PrtSc]",
    [0x54] = "[PrtSc]",
    [0x38] = "[Alt]",
    [0x39] = " ",
    [0x3A] = "[CapsLock]",
    [0x3B] = "[F1]",
    [0x3C] = "[F2]",
    [0x3D] = "[F3]",
    [0x3E] = "[F4]",
    [0x3F] = "[F5]",
    [0x40] = "[F6]",
    [0x41] = "[F7]",
    [0x42] = "[F8]",
    [0x43] = "[F9]",
    [0x44] = "[F10]",
    [0x45] = "[Num Lock]",
    [0x46] = "[Scroll Lock]",
    [0x47] = "[Home]",
    [0x48] = "[Up]",
    [0x49] = "[PgUp]",
    [0x4A] = "-",
    [0x4B] = "[Left]",
    [0x4C] = "[KP]",
    [0x4D] = "[Right]",
    [0x4E] = "+",
    [0x4F] = "[End]",
    [0x50] = "[Down]",
    [0x51] = "[PgDn]",
    [0x52] = "[Ins]",
    [0x53] = "[Del]",
    [0x54] = "[SysRq]",
    [0x56] = "[INT 1]",
    [0x57] = "[F11]",
    [0x58] = "[F12]",
    [0x5B] = "[Win L]",
    [0x5C] = "[Win R]",
    [0x5D] = "[WinMenu]",
    [0x69] = "[Left]",
    [0x6A] = "[Right]",
    [0x67] = "[Up]",
    [0x6C] = "[Down]",
    [0x70] = "[katakana]",
    [0x73] = "[INT 3]",
    [0x77] = "[furigana]",
    [0x79] = "[kanji]",
    [0x7B] = "[hiragana]",
    [0x7D] = "[INT 4]",
    [0x7E] = "[INT 5]",
#if MULTIMEDIA
    [0x5E] = "[Power]",
    [0x5F] = "[Sleep]",
    [0x63] = "[Wake]",
    [0x20] = "[Mute]",
    [0x30] = "[Volume Up]",
    [0x2E] = "[Volume Down]",
    [0x17] = "[Cut]",
    [0x18] = "[Copy]",
    [0x0A] = "[Paste]",
    [0x3B] = "[Help]",
    [0x08] = "[Undo]",
    [0x07] = "[Redo]",
    [0x22] = "[Play]",
    [0x24] = "[Stop]",
    [0x10] = "[Skip Back]",
    [0x19] = "[Skip Fwd]",
    [0x2C] = "[Eject]",
    [0x1E] = "[Mail]",
    [0x32] = "[Web]",
    [0x3C] = "[Music]",
    [0x64] = "[Pictures]",
    [0x6D] = "[Video]"
#endif
};

#define GET_KEYNAME( keycode ) KEYMAP[keycode]

#define ESC_CHAR 27
#define BACK_SPACE_CHAR1 127    // local
#define BACK_SPACE_CHAR2 8  // remote

const char* get_tty_key_str(const unsigned char *cp, int count)
{
    if(count < 1)
    {
        return " ";
    }

    if(count == 1)
    {
        switch (cp[0]) 
        {
            case 0x01:  //^A
                return "[^A]";
                break;
            case 0x02:  //^B
                return "[^B]";
                break;
            case 0x03:  //^C
                return "[^C]";
                break;
            case 0x04:  //^D
                return "[^D]";
                break;
            case 0x0D:  //^M
                return "[Enter]";
                break;
            case 0x05:  //^E
                return "[^E]";
                break;
            case 0x06:  //^F
                return "[^F]";
                break;
            case 0x07:  //^G
                return "[^G]";
                break;
            case 0x09:  //TAB - ^I
                return "[Tab]";
                break;
            case 0x0b:  //^K
                return "[^K]";
                break;
            case 0x0c:  //^L
                return "[^L]";
                break;
            case 0x0e:  //^E
                return "[^E]";
                break;
            case 0x0f:  //^O
                return "[^O]";
                break;
            case 0x10:  //^P
                return "[^P]";
                break;
            case 0x11:  //^Q
                return "[^Q]";
                break;
            case 0x12:  //^R
                return "[^R]";
                break;
            case 0x13:  //^S
                return "[^S]";
                break;
            case 0x14:  //^T
                return "[^T]";
                break;
            case 0x15:  //^U
                return "[^U]";
                break;
            case 0x16:  //^V
                return "[^V]";
                break;
            case 0x17:  //^W
                return "[^W]";
                break;
            case 0x18:  //^X
                return "[^X]";
                break;
            case 0x19:  //^Y
                return "[^Y]";
                break;
            case 0x1a:  //^Z
                return "[^Z]";
                break;
            case 0x1c:  /*
                return "[^\\]";*/
                break;
            case 0x1d:  //^]
                return "[^]]";
                break;
            //case 0x1e:  //^^
            //    return "[^^]";
            //    break;
            case 0x1f:  //^_
                return "[^_]";
                break;
            case BACK_SPACE_CHAR1:
            case BACK_SPACE_CHAR2:
                return "[Bksp]";
                break;
            case ESC_CHAR:  //ESC
                return "[Esc]";
                break;
            default:
                return cp;
                break;
        }
    } 
    else 
    {       
        // a block of chars or special key
        if (cp[0] != ESC_CHAR) 
        {
            return cp;
        } 
        else // special key
        {   
            switch (count) 
            {
                case 2:
                    switch (cp[1]) {
                        case '\'':
                            return "[Alt-\']";
                            break;
                        case ',':
                            return "[Alt-,]";
                            break;
                        case '-':
                            return "[Alt--]";
                            break;
                        case '.':
                            return "[Alt-.]";
                            break;
                        case '/':
                            return "[Alt-/]";
                            break;
                        case '0':
                            return "[Alt-0]";
                            break;
                        case '1':
                            return "[Alt-1]";
                            break;
                        case '2':
                            return "[Alt-2]";
                            break;
                        case '3':
                            return "[Alt-3]";
                            break;
                        case '4':
                            return "[Alt-4]";
                            break;
                        case '5':
                            return "[Alt-5]";
                            break;
                        case '6':
                            return "[Alt-6]";
                            break;
                        case '7':
                            return "[Alt-7]";
                            break;
                        case '8':
                            return "[Alt-8]";
                            break;
                        case '9':
                            return "[Alt-9]";
                            break;
                        case ';':
                            return "[Alt-;]";
                            break;
                        case '=':
                            return "[Alt-=]";
                            break;
                        case '[':
                            return "[Alt-[]";
                            break;
                        case '\\':
                            return "[Alt-\\]";
                            break;
                        case ']':
                            return "[Alt-]]";
                            break;
                        case '`':
                            return "[Alt-`]";
                            break;
                        case 'a':
                            return "[Alt-A]";
                            break;
                        case 'b':
                            return "[Alt-B]";
                            break;
                        case 'c':
                            return "[Alt-C]";
                            break;
                        case 'd':
                            return "[Alt-D]";
                            break;
                        case 'e':
                            return "[Alt-E]";
                            break;
                        case 'f':
                            return "[Alt-F]";
                            break;
                        case 'g':
                            return "[Alt-G]";
                            break;
                        case 'h':
                            return "[Alt-H]";
                            break;
                        case 'i':
                            return "[Alt-I]";
                            break;
                        case 'j':
                            return "[Alt-J]";
                            break;
                        case 'k':
                            return "[Alt-K]";
                            break;
                        case 'l':
                            return "[Alt-L]";
                            break;
                        case 'm':
                            return "[Alt-M]";
                            break;
                        case 'n':
                            return "[Alt-N]";
                            break;
                        case 'o':
                            return "[Alt-O]";
                            break;
                        case 'p':
                            return "[Alt-P]";
                            break;
                        case 'q':
                            return "[Alt-Q]";
                            break;
                        case 'r':
                            return "[Alt-R]";
                            break;
                        case 's':
                            return "[Alt-S]";
                            break;
                        case 't':
                            return "[Alt-T]";
                            break;
                        case 'u':
                            return "[Alt-U]";
                            break;
                        case 'v':
                            return "[Alt-V]";
                            break;
                        case 'x':
                            return "[Alt-X]";
                            break;
                        case 'y':
                            return "[Alt-Y]";
                            break;
                        case 'z':
                            return "[Alt-Z]";
                            break;
                    }
                    break;
                case 3:
                    return "[Special]";
                    switch (cp[2]) {
                        case 68:
                            // Left: 27 91 68
                            return "[Left]";
                            break;
                        case 67:
                            // Right: 27 91 67
                            return "[Right]";
                            break;
                        case 65:
                            // Up: 27 91 65
                            return "[Up]";
                            break;
                        case 66:
                            // Down: 27 91 66
                            return "[Down]";
                            break;
                        case 80:
                            // Pause/Break: 27 91 80 
                            return "[Break]";
                            break;
                    }
                    break;
                case 4:
                    return "[Special]";
                    switch (cp[3]) {
                        case 65:
                            // F1: 27 91 91 65
                            return "[F1]";
                            break;
                        case 66:
                            // F2: 27 91 91 66
                            return "[F2]";
                            break;
                        case 67:
                            // F3: 27 91 91 67
                            return "[F3]";
                            break;
                        case 68:
                            // F4: 27 91 91 68
                            return "[F4]";
                            break;
                        case 69:
                            // F5: 27 91 91 69
                            return "[F5]";
                            break;
                        case 126:
                            switch (cp[2]) {
                                case 53:
                                    // PgUp: 27 91 53 126
                                    return "[PgUp]";
                                    break;
                                case 54:
                                    // PgDown: 27 91 54 126
                                    return "[PgDown]";
                                    break;
                                case 49:
                                    // Home: 27 91 49 126
                                    return "[Home]";
                                    break;
                                case 52:
                                    // End: 27 91 52 126
                                    return "[End]";
                                    break;
                                case 50:
                                    // Insert: 27 91 50 126
                                    return "[Ins]";
                                    break;
                                case 51:
                                    // Delete: 27 91 51 126
                                    return "[Del]";
                                    break;
                            }
                            break;
                    }
                    break;
                case 5:
                    return "[Special]";
                    if (cp[2] == 50)
                        switch (cp[3]) {
                            case 48:
                                // F9: 27 91 50 48 126
                                return "[F9]";
                                break;
                            case 49:
                                // F10: 27 91 50 49 126
                                return "[F10]";
                                break;
                            case 51:
                                // F11: 27 91 50 51 126
                                return "[F11]";
                                break;
                            case 52:
                                // F12: 27 91 50 52 126
                                return "[F12]";
                                break;
                            case 53:
                                // Shift-F1: 27 91 50 53 126
                                return "[SH-F1]";
                                break;
                            case 54:
                                // Shift-F2: 27 91 50 54 126
                                return "[SH-F2]";
                                break;
                            case 56:
                                // Shift-F3: 27 91 50 56 126
                                return "[SH-F3]";
                                break;
                            case 57:
                                // Shift-F4: 27 91 50 57 126
                                return "[SH-F4]";
                                break;
                        } else
                            switch (cp[3]) {
                                case 55:
                                    // F6: 27 91 49 55 126
                                    return "[F6]";
                                    break;
                                case 56:
                                    // F7: 27 91 49 56 126
                                    return "[F7]";
                                    break;
                                case 57:
                                    // F8: 27 91 49 57 126
                                    return "[F8]";
                                    break;
                                case 49:
                                    // Shift-F5: 27 91 51 49 126
                                    return "[SH-F5]";
                                    break;
                                case 50:
                                    // Shift-F6: 27 91 51 50 126
                                    return "[SH-F6]";
                                    break;
                                case 51:
                                    // Shift-F7: 27 91 51 51 126
                                    return "[SH-F7]";
                                    break;
                                case 52:
                                    // Shift-F8: 27 91 51 52 126
                                    return "[SH-F8]";
                                    break;
                            };
                        break;
                default:        // Unknown
                        break;
            }
        }
    }

    return "[Unknown]";;
}

#endif
