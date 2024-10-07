#define ___ &trans
#define XXX &none

#ifndef QUICK_TAP_MS
  #define QUICK_TAP_MS 162
#endif

#define ZMK_HELPER_STRINGIFY(x) #x

#define MO_TOG(layer) &mo_tog layer layer   // Macro to apply momentary-layer-on-hold/toggle-layer-on-tap to a specific layer

&sk {  // sticky-key config
    release-after-ms = <900>;  // release after 0.6s
    quick-release;             // no double capitalization when rolling keys
};

&sl {  // sticky-layer config
    ignore-modifiers;          // allow chording sticky mods & layers
};

&lt {  // layer-tap config
    flavor = "balanced";
    tapping-term-ms = <200>;
    quick-tap-ms = <QUICK_TAP_MS>;
};

// Norwegian specific keycodes
#define NB_GT    LS(NON_US_BSLH)
#define NB_LT    NON_US_BSLH
#define NB_DLLR  RA(N4)
#define NB_AT    RA(N2)
#define NB_BSLH  EQUAL
#define NB_MINUS FSLH
#define NB_UNDER LS(NB_MINUS)
#define NB_PIPE  GRAVE
#define NB_PLUS  MINUS
#define NB_EQUAL RPAR
#define NB_STAR  PIPE
#define NB_CARET RBRC
#define NB_AMPS  CARET
#define NB_FSLH  LS(N7)
#define NB_QST   LS(MINUS)
#define NB_DQT   LS(N2)
#define NB_SQT   NON_US_HASH
#define NB_LTICK LS(EQUAL)
#define NB_RTICK RA(EQUAL)
#define NB_TILD  RA(RBKT)

#define NB_LPAR  LS(N8)
#define NB_RPAR  LS(N9)
#define NB_LBKT  RA(N8)
#define NB_RBKT  RA(N9)
#define NB_LBRC  RA(N7)
#define NB_RBRC  RA(N0)

// Some general behaviours that are slightly tweaked from the core
/ {
    behaviours {
        kp_mo_tap: behavior_kp_mo_tap {
	    compatible = "zmk,behavior-hold-tap";
	    label = "kp_mo_tap";
	    #binding-cells = <2>;
	    flavor = "tap-preferred";
	    tapping-term-ms = <175>;
	    quick-tap-ms = <175>;
	    //require-prior-idle-ms = <140>;
	    bindings = <&mo>, <&kp>;
	};
        kp_mo_hold: behavior_kp_mo_hold {
	    compatible = "zmk,behavior-hold-tap";
	    label = "kp_mo_hold";
	    #binding-cells = <2>;
	    flavor = "hold-preferred";
	    tapping-term-ms = <175>;
	    quick-tap-ms = <175>;
	    //require-prior-idle-ms = <140>;
	    bindings = <&mo>, <&kp>;
	};
	kp_mt: behavior_kp_mt {
	    compatible = "zmk,behavior-hold-tap";
	    label = "kp_mt";
	    #binding-cells = <2>;
	    flavor = "tap-preferred";
	    tapping-term-ms = <QUICK_TAP_MS>;
	    quick-tap-ms = <QUICK_TAP_MS>;
	    //require-prior-idle-ms = <100>;
	    bindings = <&kp>, <&kp>;
	};
      };
};


#include "homerow.h"
#include "unicode.h"

