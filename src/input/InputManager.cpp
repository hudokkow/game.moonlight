/*
 *      Copyright (C) 2015 Team XBMC
 *      http://xbmc.org
 *
 *  This Program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *
 *  This Program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with XBMC; see the file COPYING.  If not, see
 *  <http://www.gnu.org/licenses/>.
 *
 */

#include "MoonlightEnvironment.h"
#include "InputManager.h"
#include "Limelight.h"
#include "log/Log.h"

#include "kodi/libKODI_game.h"

#define PORT_NUM 0

using namespace MOONLIGHT;

CInputManager& CInputManager::Get(void)
{
  static CInputManager _instance;
  return _instance;
}

bool CInputManager::OpenPort()
{
  if (!CMoonlightEnvironment::Get().GetFrontend())
    return false;

  m_port_opened = true;

  CMoonlightEnvironment::Get().GetFrontend()->OpenPort(PORT_NUM);

  return true;
}

void CInputManager::ClosePort()
{
  if (!CMoonlightEnvironment::Get().GetFrontend())
    return;

  CMoonlightEnvironment::Get().GetFrontend()->ClosePort(PORT_NUM);

  m_port_opened = false;
}

int CInputManager::GetIndex(const std::string& strControllerId, const std::string& strFeatureName)
{
  // Handle default controller
  if (strControllerId == "game.controller.default")
  {
    if (strFeatureName == "a")            return A_FLAG;
    if (strFeatureName == "b")            return B_FLAG;
    if (strFeatureName == "x")            return X_FLAG;
    if (strFeatureName == "y")            return Y_FLAG;
    if (strFeatureName == "start")        return PLAY_FLAG;
    if (strFeatureName == "back")         return BACK_FLAG;
    if (strFeatureName == "leftbumper")   return LB_FLAG;
    if (strFeatureName == "rightbumper")  return RB_FLAG;
    if (strFeatureName == "leftthumb")    return LS_CLK_FLAG;
    if (strFeatureName == "rightthumb")   return RS_CLK_FLAG;
    if (strFeatureName == "up")           return UP_FLAG;
    if (strFeatureName == "down")         return DOWN_FLAG;
    if (strFeatureName == "right")        return RIGHT_FLAG;
    if (strFeatureName == "left")         return LEFT_FLAG;
  }
  return -1;
}

bool CInputManager::InputEvent(unsigned int port, const game_input_event& event)
{
  const std::string strControllerId = event.controller_id;
  const std::string strFeatureName = event.feature_name;

  if(!m_port_opened) {
	return false;
  }

  switch (event.type)
  {
  case GAME_INPUT_EVENT_DIGITAL_BUTTON: {
	  int button = 0;
	  int index = GetIndex(strControllerId, strFeatureName);
	  if (event.digital_button.pressed)
		  button |= index;
	  else
		  button &= ~index;
	  LiSendControllerEvent(button, 0, 0, 0, 0, 0, 0);
	  break;
  }
  case GAME_INPUT_EVENT_ANALOG_BUTTON:
	  break;

  case GAME_INPUT_EVENT_ANALOG_STICK:
	  break;

  case GAME_INPUT_EVENT_ACCELEROMETER:
	  break;

  case GAME_INPUT_EVENT_KEY:
  {
	  char action = event.key.pressed ? KEY_ACTION_DOWN : KEY_ACTION_UP;
	  LiSendKeyboardEvent((short)event.key.character, action, event.key.modifiers);
	  break;
  }

  case GAME_INPUT_EVENT_RELATIVE_POINTER:
	  break;

  case GAME_INPUT_EVENT_ABSOLUTE_POINTER:
	  break;

  default:
	  break;
  }
  return true;
}