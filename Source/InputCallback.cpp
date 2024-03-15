/// @file InputCallback.cpp
/// @author Tyler Birdsall (tyler.birdsall@digipen.edu)
/// @brief Input system manual callback and key display, since glfw and imgui hate each other
/// @version 0.1
/// @date 2024-22-02
/// 
/// @copyright Copyright (c) 2023


#include "pch.h" // precompiled header has to be included first

#include "InputSystem.h"


/// @brief  gets the name of a GLFW keyboard button
/// @param  glfwId  the ID of the keyboard button to get the name of
/// @return the name of the keyboard button
const char* InputSystem::GetKeyboardButtonName( int glfwId ) const
{
    static std::map< int, char const* > const keyNames = {
        { GLFW_KEY_UNKNOWN      , "UNKNOWN"  },
        { GLFW_KEY_SPACE        , "SPACE"    },
        { GLFW_KEY_APOSTROPHE   , "'"        },
        { GLFW_KEY_COMMA        , ","        },
        { GLFW_KEY_MINUS        , "-"        },
        { GLFW_KEY_PERIOD       , "."        },
        { GLFW_KEY_SLASH        , "/"        },
        { GLFW_KEY_0            , "0"        },
        { GLFW_KEY_1            , "1"        },
        { GLFW_KEY_2            , "2"        },
        { GLFW_KEY_3            , "3"        },
        { GLFW_KEY_4            , "4"        },
        { GLFW_KEY_5            , "5"        },
        { GLFW_KEY_6            , "6"        },
        { GLFW_KEY_7            , "7"        },
        { GLFW_KEY_8            , "8"        },
        { GLFW_KEY_9            , "9"        },
        { GLFW_KEY_SEMICOLON    , ":"        },
        { GLFW_KEY_EQUAL        , "="        },
        { GLFW_KEY_A            , "A"        },
        { GLFW_KEY_B            , "B"        },
        { GLFW_KEY_C            , "C"        },
        { GLFW_KEY_D            , "D"        },
        { GLFW_KEY_E            , "E"        },
        { GLFW_KEY_F            , "F"        },
        { GLFW_KEY_G            , "G"        },
        { GLFW_KEY_H            , "H"        },
        { GLFW_KEY_I            , "I"        },
        { GLFW_KEY_J            , "J"        },
        { GLFW_KEY_K            , "K"        },
        { GLFW_KEY_L            , "L"        },
        { GLFW_KEY_M            , "M"        },
        { GLFW_KEY_N            , "N"        },
        { GLFW_KEY_O            , "O"        },
        { GLFW_KEY_P            , "P"        },
        { GLFW_KEY_Q            , "Q"        },
        { GLFW_KEY_R            , "R"        },
        { GLFW_KEY_S            , "S"        },
        { GLFW_KEY_T            , "T"        },
        { GLFW_KEY_U            , "U"        },
        { GLFW_KEY_V            , "V"        },
        { GLFW_KEY_W            , "W"        },
        { GLFW_KEY_X            , "X"        },
        { GLFW_KEY_Y            , "Y"        },
        { GLFW_KEY_Z            , "Z"        },
        { GLFW_KEY_LEFT_BRACKET , "["        },
        { GLFW_KEY_BACKSLASH    , "\\"       },
        { GLFW_KEY_RIGHT_BRACKET, "]"        },
        { GLFW_KEY_GRAVE_ACCENT , "`"        },
        { GLFW_KEY_WORLD_1      , "-1"       },
        { GLFW_KEY_WORLD_2      , "-2"       },
        { GLFW_KEY_ESCAPE       , "ESC"      },
        { GLFW_KEY_ENTER        , "ENTER"    },
        { GLFW_KEY_TAB          , "TAB"      },
        { GLFW_KEY_BACKSPACE    , "BACKSPACE"},
        { GLFW_KEY_INSERT       , "INSERT"   },
        { GLFW_KEY_DELETE       , "DEL"      },
        { GLFW_KEY_RIGHT        , "RIGHT"    },
        { GLFW_KEY_LEFT         , "LEFT"     },
        { GLFW_KEY_DOWN         , "DOWN"     },
        { GLFW_KEY_UP           , "UP"       },
        { GLFW_KEY_PAGE_UP      , "PG-UP"    },
        { GLFW_KEY_PAGE_DOWN    , "PG-DOWN"  },
        { GLFW_KEY_HOME         , "HOME"     },
        { GLFW_KEY_END          , "END"      },
        { GLFW_KEY_CAPS_LOCK    , "CAPS"     },
        { GLFW_KEY_SCROLL_LOCK  , "SCRL-LK"  },
        { GLFW_KEY_NUM_LOCK     , "NUM-LK"   },
        { GLFW_KEY_PRINT_SCREEN , "PRT-SC"   },
        { GLFW_KEY_PAUSE        , "PAUSE"    },
        { GLFW_KEY_F1           , "F1"       },
        { GLFW_KEY_F2           , "F2"       },
        { GLFW_KEY_F3           , "F3"       },
        { GLFW_KEY_F4           , "F4"       },
        { GLFW_KEY_F5           , "F5"       },
        { GLFW_KEY_F6           , "F6"       },
        { GLFW_KEY_F7           , "F7"       },
        { GLFW_KEY_F8           , "F8"       },
        { GLFW_KEY_F9           , "F9"       },
        { GLFW_KEY_F10          , "F10"      },
        { GLFW_KEY_F11          , "F11"      },
        { GLFW_KEY_F12          , "F12"      },
        { GLFW_KEY_F13          , "F13"      },
        { GLFW_KEY_F14          , "F14"      },
        { GLFW_KEY_F15          , "F15"      },
        { GLFW_KEY_F16          , "F16"      },
        { GLFW_KEY_F17          , "F17"      },
        { GLFW_KEY_F18          , "F18"      },
        { GLFW_KEY_F19          , "F19"      },
        { GLFW_KEY_F20          , "F20"      },
        { GLFW_KEY_F21          , "F21"      },
        { GLFW_KEY_F22          , "F22"      },
        { GLFW_KEY_F23          , "F23"      },
        { GLFW_KEY_F24          , "F24"      },
        { GLFW_KEY_F25          , "F25"      },
        { GLFW_KEY_KP_0         , "NUM0"     },
        { GLFW_KEY_KP_1         , "NUM1"     },
        { GLFW_KEY_KP_2         , "NUM2"     },
        { GLFW_KEY_KP_3         , "NUM3"     },
        { GLFW_KEY_KP_4         , "NUM4"     },
        { GLFW_KEY_KP_5         , "NUM5"     },
        { GLFW_KEY_KP_6         , "NUM6"     },
        { GLFW_KEY_KP_7         , "NUM7"     },
        { GLFW_KEY_KP_8         , "NUM8"     },
        { GLFW_KEY_KP_9         , "NUM9"     },
        { GLFW_KEY_KP_DECIMAL   , "NUM."     },
        { GLFW_KEY_KP_DIVIDE    , "NUM/"     },
        { GLFW_KEY_KP_MULTIPLY  , "NUM*"     },
        { GLFW_KEY_KP_SUBTRACT  , "NUM-"     },
        { GLFW_KEY_KP_ADD       , "NUM+"     },
        { GLFW_KEY_KP_ENTER     , "NUMENTER" },
        { GLFW_KEY_KP_EQUAL     , "NUM="     },
        { GLFW_KEY_LEFT_SHIFT   , "LSHIFT"   },
        { GLFW_KEY_LEFT_CONTROL , "LCTRL"    },
        { GLFW_KEY_LEFT_ALT     , "LALT"     },
        { GLFW_KEY_LEFT_SUPER   , "LSUPER"   },
        { GLFW_KEY_RIGHT_SHIFT  , "RSHIFT"   },
        { GLFW_KEY_RIGHT_CONTROL, "RCTRL"    },
        { GLFW_KEY_RIGHT_ALT    , "RALT"     },
        { GLFW_KEY_RIGHT_SUPER  , "RSUPER"   },
        { GLFW_KEY_MENU         , "MENU"     }
    };

    auto itr = keyNames.find( glfwId );
    if ( itr == keyNames.end() )
    {
        return "ERRKEY";
    }

    return itr->second;
}

/// @brief  gets the name of a GLFW mouse button
/// @param  glfwId  the ID of the mouse button to get the name of
/// @return the name of the mouse button
char const* InputSystem::GetMouseButtonName( int glfwId ) const
{
    static std::map< int, char const* > const keyNames = {
        { GLFW_MOUSE_BUTTON_1, "LMB" },
        { GLFW_MOUSE_BUTTON_2, "RMB" },
        { GLFW_MOUSE_BUTTON_3, "MMB" },
        { GLFW_MOUSE_BUTTON_4, "M4"  },
        { GLFW_MOUSE_BUTTON_5, "M5"  },
        { GLFW_MOUSE_BUTTON_6, "M6"  },
        { GLFW_MOUSE_BUTTON_7, "M7"  },
        { GLFW_MOUSE_BUTTON_8, "M8"  }
    };

    auto itr = keyNames.find( glfwId );
    if ( itr == keyNames.end() )
    {
        return "ERRKEY";
    }

    return itr->second;
}

/// @brief  gets the name of a GLFW controller button
/// @param  glfwId  the ID of the controller button to get the name of
/// @return the name of the controller button
char const* InputSystem::GetControllerButtonName( int glfwId ) const
{
    static std::map< int, char const* > const keyNames = {
        { GLFW_GAMEPAD_BUTTON_A           , "A"            },
        { GLFW_GAMEPAD_BUTTON_B           , "B"            },
        { GLFW_GAMEPAD_BUTTON_X           , "X"            },
        { GLFW_GAMEPAD_BUTTON_Y           , "Y"            },
        { GLFW_GAMEPAD_BUTTON_LEFT_BUMPER , "LEFT_BUMPER"  },
        { GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER, "RIGHT_BUMPER" },
        { GLFW_GAMEPAD_BUTTON_BACK        , "BACK"         },
        { GLFW_GAMEPAD_BUTTON_START       , "START"        },
        { GLFW_GAMEPAD_BUTTON_GUIDE       , "GUIDE"        },
        { GLFW_GAMEPAD_BUTTON_LEFT_THUMB  , "LEFT_THUMB"   },
        { GLFW_GAMEPAD_BUTTON_RIGHT_THUMB , "RIGHT_THUMB"  },
        { GLFW_GAMEPAD_BUTTON_DPAD_UP     , "DPAD_UP"      },
        { GLFW_GAMEPAD_BUTTON_DPAD_RIGHT  , "DPAD_RIGHT"   },
        { GLFW_GAMEPAD_BUTTON_DPAD_DOWN   , "DPAD_DOWN"    },
        { GLFW_GAMEPAD_BUTTON_DPAD_LEFT   , "DPAD_LEFT"    }
    };

    auto itr = keyNames.find( glfwId );
    if ( itr == keyNames.end() )
    {
        return "ERRKEY";
    }

    return itr->second;
}

/// @brief  gets the name of a GLFW controller axis
/// @param  glfwId  the ID of the controller axis to get the name of
/// @return the name of the controller axis
char const* InputSystem::GetControllerAxisName( int glfwId ) const
{
    static std::map< int, char const* > const keyNames = {
        { GLFW_GAMEPAD_AXIS_LEFT_X       , "LEFT_X"        },
        { GLFW_GAMEPAD_AXIS_LEFT_Y       , "LEFT_Y"        },
        { GLFW_GAMEPAD_AXIS_RIGHT_X      , "RIGHT_X"       },
        { GLFW_GAMEPAD_AXIS_RIGHT_Y      , "RIGHT_Y"       },
        { GLFW_GAMEPAD_AXIS_LEFT_TRIGGER , "LEFT_TRIGGER"  },
        { GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER, "RIGHT_TRIGGER" }
    };

    auto itr = keyNames.find( glfwId );
    if ( itr == keyNames.end() )
    {
        return "ERRKEY";
    }

    return itr->second;
}
