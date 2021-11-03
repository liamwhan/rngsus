//
//  rng_gui.h
//
//  Created by Liam Whan on 4/11/2021.
//

#if !defined(RNG_GUI_H)
#define RNG_GUI_H

void InitUI(ui_state *State);
void PushButtonTextColour();
void PopButtonTextColour();
ui_state InitUiState();
void RenderUi(ui_state *State);
void ShutdownUi();

#endif /* RNG_GUI_H */
