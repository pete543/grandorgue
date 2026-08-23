/*
 * Copyright 2006 Milan Digital Audio LLC
 * Copyright 2009-2025 GrandOrgue contributors (see AUTHORS)
 * License GPL-2.0 or later
 * (https://www.gnu.org/licenses/old-licenses/gpl-2.0.html).
 */

#include "GOCallbackButtonControl.h"

#include <wx/intl.h>

#include "control/GOButtonCallback.h"

GOCallbackButtonControl::GOCallbackButtonControl(
  GOOrganModel &organModel,
  GOButtonCallback *callback,
  bool isPushbutton,
  bool isPiston,
  bool isHoldable)
  : GOButtonControl(
    organModel, OBJECT_TYPE_BUTTON, MIDI_RECV_SETTER, isPushbutton, isPiston),
    m_callback(callback),
    m_IsHoldable(isHoldable) {}

void GOCallbackButtonControl::Push() {
  if (m_IsHoldable) {
    Press();
    Release();
  } else if (m_Pushbutton)
    m_callback->ButtonStateChanged(this, true);
  else
    GOButtonControl::Push();
}

void GOCallbackButtonControl::Press() {
  if (m_IsHoldable)
    SetButtonState(true);
  else
    Push();
}

void GOCallbackButtonControl::Release() {
  if (m_IsHoldable)
    SetButtonState(false);
}

void GOCallbackButtonControl::OnMidiReceived(
  const GOMidiEvent &event, GOMidiMatchType matchType, int key, int value) {
  if (m_IsHoldable) {
    if (matchType == MIDI_MATCH_ON)
      Press();
    else if (matchType == MIDI_MATCH_OFF)
      Release();
    else if (matchType == MIDI_MATCH_CHANGE)
      Push();
  } else
    GOButtonControl::OnMidiReceived(event, matchType, key, value);
}

void GOCallbackButtonControl::SetButtonState(bool on) {
  if (IsEngaged() == on)
    return;
  m_callback->ButtonStateChanged(this, on);
  Display(on);
}
