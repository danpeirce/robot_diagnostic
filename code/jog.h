#include "command_list.h"

#ifndef JOG_MSG
#define JOG_MSG

enum jog_msg {j_null=0, jog_spin_left_med, jog_spin_left_fast, 
                jog_spin_right_med, jog_spin_right_fast, done};


enum commands jog1_state( void );
enum commands jog2_state( void );

#endif
