#ifndef PACK_H
#define PACK_H

#include "power.h"

void try_pack(INP struct s_packer_opts *packer_opts, INP const t_arch * arch, INP t_model *user_models, INP t_model *library_models, t_power_opts * power_opts);

#endif
