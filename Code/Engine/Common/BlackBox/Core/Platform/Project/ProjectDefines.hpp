#pragma once

#if defined(IS_MONOLITHIC_BUILD)
	#define SYS_ENV_AS_STRUCT
#endif

#define NOT_USE_UBICOM_SDK					// mainly needed for licencees to compile without the UBI.com integration

#define NOT_USE_PUNKBUSTER_SDK				// mainly needed for licencees to compile without the Punkbuster integration
#define NOT_USE_BINK_SDK					// mainly needed for licencees to compile without the Bink integration
#define NOT_USE_DIVX_SDK					// mainly needed for licencees to compile without the DivX integration
#define NOT_USE_ASE_SDK						// mainly needed for licencees to compile without the ASE integration
#define EXCLUDE_UBICOM_CLIENT_SDK			// to compile a standalone server without the client integration

