#pragma once

#define MAKE_HRM(NAME, HOLD, TAP) \
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
	  hold-trigger-on-release; \
	}; \
      }; \
    };

MAKE_HRM(hml, &kp, &kp)
MAKE_HRM(hmr, &kp, &kp)

