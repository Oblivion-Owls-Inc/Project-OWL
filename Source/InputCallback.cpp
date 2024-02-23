/// @file InputCallback.cpp
/// @author Tyler Birdsall (tyler.birdsall@digipen.edu)
/// @brief Input system manual callback and key display, since glfw and imgui hate each other
/// @version 0.1
/// @date 2024-22-02
/// 
/// @copyright Copyright (c) 2023

#include "InputSystem.h"
#include "PlatformSystem.h"
#include <map>
#include "CameraSystem.h"
#include "Engine.h"
#include "DebugSystem.h"

/// @brief  manual callback catch for keys since imgui hates glfw
void InputSystem::Action::ManualKeyCallback()
{
    GLFWwindow* handle = Input()->GetHandle();
	bool found = false;
	int key = 0;
    if (!found)
    {
        // -1 handles unknown key
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
        int whichAction = Input()->M_changeingAction;
        switch (whichAction)
        {
        case 1:
            Input()->GetActionByName(Input()->M_whichAction)->AddKeyInput(key);
            break;
        case 2:
            Input()->GetActionByName(Input()->M_whichAction)->RemoveKeyInput(key);
            break;
        case 7:
            Input()->GetActionByName(Input()->M_whichAction)->AddKeyAxisNegative(key);
            break;
        case 8:
            Input()->GetActionByName(Input()->M_whichAction)->RemoveKeyAxisNegative(key);
            break;
        default:
            break;
        }
        Input()->M_changeingAction = 0;
	}
}

/// @brief  displays a text version of a key to imgui (65 becomes "A", etc)
/// @param  key to convert
void InputSystem::keyVisual(int key)
{
    switch (key)
    {
    case -1:
        ImGui::Text("UNKNOWN");
        break;
    case 32:
        ImGui::Text("SPACE");
        break;
    case 39:
        ImGui::Text("'");
        break;
    case 44:
        ImGui::Text(",");
        break;
    case 45:
        ImGui::Text("-");
        break;
    case 46:
        ImGui::Text(".");
        break;
    case 47:
        ImGui::Text("/");
        break;
    case 48:
        ImGui::Text("0");
        break;
    case 49:
        ImGui::Text("1");
        break;
    case 50:
        ImGui::Text("2");
        break;
    case 51:
        ImGui::Text("3");
        break;
    case 52:
        ImGui::Text("4");
        break;
    case 53:
        ImGui::Text("5");
        break;
    case 54:
        ImGui::Text("6");
        break;
    case 55:
        ImGui::Text("7");
        break;
    case 56:
        ImGui::Text("8");
        break;
    case 57:
        ImGui::Text("9");
        break;
    case 59:
        ImGui::Text(":");
        break;
    case 61:
        ImGui::Text("=");
        break;
    case 65:
        ImGui::Text("A");
        break;
    case 66:
        ImGui::Text("B");
        break;
    case 67:
        ImGui::Text("C");
        break;
    case 68:
        ImGui::Text("D");
        break;
    case 69:
        ImGui::Text("E");
        break;
    case 70:
        ImGui::Text("F");
        break;
    case 71:
        ImGui::Text("G");
        break;
    case 72:
        ImGui::Text("H");
        break;
    case 73:
        ImGui::Text("I");
        break;
    case 74:
        ImGui::Text("J");
        break;
    case 75:
        ImGui::Text("K");
        break;
    case 76:
        ImGui::Text("L");
        break;
    case 77:
        ImGui::Text("M");
        break;
    case 78:
        ImGui::Text("N");
        break;
    case 79:
        ImGui::Text("O");
        break;
    case 80:
        ImGui::Text("P");
        break;
    case 81:
        ImGui::Text("Q");
        break;
    case 82:
        ImGui::Text("R");
        break;
    case 83:
        ImGui::Text("S");
        break;
    case 84:
        ImGui::Text("T");
        break;
    case 85:
        ImGui::Text("U");
        break;
    case 86:
        ImGui::Text("V");
        break;
    case 87:
        ImGui::Text("W");
        break;
    case 88:
        ImGui::Text("X");
        break;
    case 89:
        ImGui::Text("Y");
        break;
    case 90:
        ImGui::Text("Z");
        break;
    case 91:
        ImGui::Text("[");
        break;
    case 92:
        ImGui::Text("\\");
        break;
    case 93:
        ImGui::Text("]");
        break;
    case 96:
        ImGui::Text("`");
        break;
    case 161:
        ImGui::Text("NON-US-1");
        break;
    case 162:
        ImGui::Text("NON-US-2");
        break;
    case 256:
        ImGui::Text("ESC");
        break;
    case 257:
        ImGui::Text("ENTER");
        break;
    case 258:
        ImGui::Text("TAB");
        break;
    case 259:
        ImGui::Text("BACKSPACE");
        break;
    case 260:
        ImGui::Text("INSERT");
        break;
    case 261:
        ImGui::Text("DEL");
        break;
    case 262:
        ImGui::Text("RIGHT");
        break;
    case 263:
        ImGui::Text("LEFT");
        break;
    case 264:
        ImGui::Text("DOWN");
        break;
    case 265:
        ImGui::Text("UP");
        break;
    case 266:
        ImGui::Text("PG-UP");
        break;
    case 267:
        ImGui::Text("PG-DOWN");
        break;
    case 268:
        ImGui::Text("HOME");
        break;
    case 269:
        ImGui::Text("END");
        break;
    case 280:
        ImGui::Text("CAPS");
        break;
    case 281:
        ImGui::Text("SCRL-LK");
        break;
    case 282:
        ImGui::Text("NUM-LK");
        break;
    case 283:
        ImGui::Text("PRT-SC");
        break;
    case 284:
        ImGui::Text("PAUSE");
        break;
    case 290:
        ImGui::Text("F1");
        break;
    case 291:
        ImGui::Text("F2");
        break;
    case 292:
        ImGui::Text("F3");
        break;
    case 293:
        ImGui::Text("F4");
        break;
    case 294:
        ImGui::Text("F5");
        break;
    case 295:
        ImGui::Text("F6");
        break;
    case 296:
        ImGui::Text("F7");
        break;
    case 297:
        ImGui::Text("F8");
        break;
    case 298:
        ImGui::Text("F9");
        break;
    case 299:
        ImGui::Text("F10");
        break;
    case 300:
        ImGui::Text("F11");
        break;
    case 301:
        ImGui::Text("F12");
        break;
    case 302:
        ImGui::Text("F13");
        break;
    case 303:
        ImGui::Text("F14");
        break;
    case 304:
        ImGui::Text("F15");
        break;
    case 305:
        ImGui::Text("F16");
        break;
    case 306:
        ImGui::Text("F17");
        break;
    case 307:
        ImGui::Text("F18");
        break;
    case 308:
        ImGui::Text("F19");
        break;
    case 309:
        ImGui::Text("F20");
        break;
    case 310:
        ImGui::Text("F21");
        break;
    case 311:
        ImGui::Text("F22");
        break;
    case 312:
        ImGui::Text("F23");
        break;
    case 313:
        ImGui::Text("F24");
        break;
    case 314:
        ImGui::Text("F25");
        break;
    case 320:
        ImGui::Text("0");
        break;
    case 321:
        ImGui::Text("1");
        break;
    case 322:
        ImGui::Text("2");
        break;
    case 323:
        ImGui::Text("3");
        break;
    case 324:
        ImGui::Text("4");
        break;
    case 325:
        ImGui::Text("5");
        break;
    case 326:
        ImGui::Text("6");
        break;
    case 327:
        ImGui::Text("7");
        break;
    case 328:
        ImGui::Text("8");
        break;
    case 329:
        ImGui::Text("9");
        break;
    case 330:
        ImGui::Text(".");
        break;
    case 331:
        ImGui::Text("/");
        break;
    case 332:
        ImGui::Text("*");
        break;
    case 333:
        ImGui::Text("-");
        break;
    case 334:
        ImGui::Text("+");
        break;
    case 335:
        ImGui::Text("ENTER");
        break;
    case 336:
        ImGui::Text("=");
        break;
    case 340:
        ImGui::Text("LSHIFT");
        break;
    case 341:
        ImGui::Text("LCTRL");
        break;
    case 342:
        ImGui::Text("LALT");
        break;
    case 343:
        ImGui::Text("WINDOWS");
        break;
    case 344:
        ImGui::Text("RSHIFT");
        break;
    case 345:
        ImGui::Text("RCTRL");
        break;
    case 346:
        ImGui::Text("RALT");
        break;
    case 347:
        ImGui::Text("RSUPER");
        break;
    case 348:
        ImGui::Text("MENU");
        break;
    default:
        ImGui::Text("ERR");
        break;
    }
}