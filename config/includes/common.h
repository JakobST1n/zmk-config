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


// Some general behaviours that are slightly tweaked from the core
/ {
    behaviours {
        kp_mo_tap: behavior_kp_mo_tap {
	    compatible = "zmk,behavior-hold-tap";
	    label = "kp_mo_tap";
	    #binding-cells = <2>;
	    flavor = "tap-preferred";
	    tapping-term-ms = <200>;
	    quick-tap-ms = <200>;
	    //require-prior-idle-ms = <140>;
	    bindings = <&mo>, <&kp>;
	};
        kp_mo_hold: behavior_kp_mo_hold {
	    compatible = "zmk,behavior-hold-tap";
	    label = "kp_mo_hold";
	    #binding-cells = <2>;
	    flavor = "hold-preferred";
	    tapping-term-ms = <200>;
	    quick-tap-ms = <200>;
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

