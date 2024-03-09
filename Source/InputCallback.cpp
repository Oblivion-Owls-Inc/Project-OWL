/// @file InputCallback.cpp
/// @author Tyler Birdsall (tyler.birdsall@digipen.edu)
/// @brief Input system manual callback and key display, since glfw and imgui hate each other
/// @version 0.1
/// @date 2024-22-02
/// 
/// @copyright Copyright (c) 2023


#include "pch.h" // precompiled header has to be included first
#include "InputSystem.h"
#include "PlatformSystem.h"
#include "CameraSystem.h"
#include "Engine.h"
#include "DebugSystem.h"

/// @brief  manual callback catch for keys since imgui hates glfw
void InputSystem::Action::ManualKeyCallback()
{
    GLFWwindow* m_Handle = Input()->GetHandle();
	bool found = false;
	int key = 0;
    if (!found)
    {
        // -1 m_Handles unknown key
        for (int i = -1; i <= GLFW_KEY_LAST; ++i)
        {
            if (glfwGetKey(Input()->GetHandle(), i))
            {
                found = true;
                key = i;
                break;
            }
        }
    }
	if (found)
	{
        int whichAction = Input()->m_ChangingAction;
        switch (whichAction)
        {
        case 1:
            Input()->GetActionByName(Input()->m_WhichAction)->AddKeyInput(key);
            break;
        case 2:
            Input()->GetActionByName(Input()->m_WhichAction)->RemoveKeyInput(key);
            break;
        case 7:
            Input()->GetActionByName(Input()->m_WhichAction)->AddKeyAxisNegative(key);
            break;
        case 8:
            Input()->GetActionByName(Input()->m_WhichAction)->RemoveKeyAxisNegative(key);
            break;
        default:
            break;
        }
        Input()->m_ChangingAction = 0;
	}
}

/// @brief  displays a text version of a key to imgui (65 becomes "A", etc)
/// @param  key to convert
const char* InputSystem::GetDebugKeyName(int key) const
{
    static std::map< int, char const* > const keyNames = {
        { -1 , "UNKNOWN"  },
        { 32 , "SPACE"    },
        { 39 , "'"        },
        { 44 , ","        },
        { 45 , "-"        },
        { 46 , "."        },
        { 47 , "/"        },
        { 48 , "0"        },
        { 49 , "1"        },
        { 50 , "2"        },
        { 51 , "3"        },
        { 52 , "4"        },
        { 53 , "5"        },
        { 54 , "6"        },
        { 55 , "7"        },
        { 56 , "8"        },
        { 57 , "9"        },
        { 59 , ":"        },
        { 61 , "="        },
        { 65 , "A"        },
        { 66 , "B"        },
        { 67 , "C"        },
        { 68 , "D"        },
        { 69 , "E"        },
        { 70 , "F"        },
        { 71 , "G"        },
        { 72 , "H"        },
        { 73 , "I"        },
        { 74 , "J"        },
        { 75 , "K"        },
        { 76 , "L"        },
        { 77 , "M"        },
        { 78 , "N"        },
        { 79 , "O"        },
        { 80 , "P"        },
        { 81 , "Q"        },
        { 82 , "R"        },
        { 83 , "S"        },
        { 84 , "T"        },
        { 85 , "U"        },
        { 86 , "V"        },
        { 87 , "W"        },
        { 88 , "X"        },
        { 89 , "Y"        },
        { 90 , "Z"        },
        { 91 , "["        },
        { 92 , "\\"       },
        { 93 , "]"        },
        { 96 , "`"        },
        { 161, "-1"       },
        { 162, "-2"       },
        { 256, "ESC"      },
        { 257, "ENTER"    },
        { 258, "TAB"      },
        { 259, "BACKSPACE"},
        { 260, "INSERT"   },
        { 261, "DEL"      },
        { 262, "RIGHT"    },
        { 263, "LEFT"     },
        { 264, "DOWN"     },
        { 265, "UP"       },
        { 266, "PG-UP"      },
        { 267, "PG-DOWN"    },
        { 268, "HOME"     },
        { 269, "END"      },
        { 280, "CAPS"     },
        { 281, "SCRL-LK"      },
        { 282, "NUM-LK"      },
        { 283, "PRT-SC"      },
        { 284, "PAUSE"    },
        { 290, "F1"       },
        { 291, "F2"       },
        { 292, "F3"       },
        { 293, "F4"       },
        { 294, "F5"       },
        { 295, "F6"       },
        { 296, "F7"       },
        { 297, "F8"       },
        { 298, "F9"       },
        { 299, "F10"      },
        { 300, "F11"      },
        { 301, "F12"      },
        { 302, "F13"      },
        { 303, "F14"      },
        { 304, "F15"      },
        { 305, "F16"      },
        { 306, "F17"      },
        { 307, "F18"      },
        { 308, "F19"      },
        { 309, "F20"      },
        { 310, "F21"      },
        { 311, "F22"      },
        { 312, "F23"      },
        { 313, "F24"      },
        { 314, "F25"      },
        { 320, "0"        },
        { 321, "1"        },
        { 322, "2"        },
        { 323, "3"        },
        { 324, "4"        },
        { 325, "5"        },
        { 326, "6"        },
        { 327, "7"        },
        { 328, "8"        },
        { 329, "9"        },
        { 330, "."        },
        { 331, "/"        },
        { 332, "*"        },
        { 333, "-"        },
        { 334, "+"        },
        { 335, "ENTER"    },
        { 336, "="        },
        { 340, "LSHIFT"   },
        { 341, "LCTRL"    },
        { 342, "LALT"     },
        { 343, "WINDOWS"  },
        { 344, "RSHIFT"   },
        { 345, "RCTRL"    },
        { 346, "RALT"     },
        { 347, "RSUPER"   },
        { 348, "MENU"     }
    };

    auto itr = keyNames.find(key);
    if (itr == keyNames.end())
    {
        return "ERRKEY";
    }

    return itr->second;
}