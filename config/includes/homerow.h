#pragma once

#define MAKE_HRM(NAME, HOLD, TAP, TRIGGER_POS) \
    / { \
      behaviours { \
	NAME: NAME { \
	  label = ZMK_HELPER_STRINGIFY(ZB_ ## NAME); \
	  compatible = "zmk,behavior-hold-tap"; \
	  #binding-cells = <2>; \
	  flavor = "balanced"; \
	  tapping-term-ms = <280>; \
	  quick-tap-ms = <QUICK_TAP_MS>; \
	  require-prior-idle-ms = <150>; \
	  bindings = <HOLD>, <TAP>; \
          hold-trigger-key-positions = <TRIGGER_POS>; \
	  hold-trigger-on-release; \
	}; \
      }; \
    };
 
MAKE_HRM(hml, &kp, &kp, KEYS_R THUMBS)
MAKE_HRM(hmr, &kp, &kp, KEYS_L THUMBS)

