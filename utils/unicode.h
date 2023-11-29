#pragma once

#define COMBO(name, POS, LAYERS, BINDINGS) \
    / { \
        combos { \
            compatible = "zmk,combos"; \
            name { \
                timeout-ms = <50>; \
                key-positions = <POS>; \
                layers = <LAYERS>; \
                bindings = <BINDINGS>; \
            }; \
        }; \
    };

/* ZMK_UNICODE */
#define UC_LEAD_MACWIN  &macro_press &kp LALT      // macOS/Windows-Alt-Codes
#define UC_LEAD_LINUX   &macro_tap &kp LS(LC(U))   // Linux
#define UC_LEAD_WINDOWS &macro_tap &kp RALT &kp U  // Windows + WinCompose (default)

#define UC_TRAIL_MACWIN  &macro_release &kp LALT  // macOS/Windows-Alt-Codes
#define UC_TRAIL_LINUX   &macro_tap &kp SPACE     // Linux
#define UC_TRAIL_WINDOWS &macro_tap &kp RET       // Windows + WinCompose (default)

#define UC_MACRO_LINUX(name, unicode_bindings) \
    / { \
        macros { \
            name: name { \
                compatible = "zmk,behavior-macro"; \
                label = ZMK_HELPER_STRINGIFY(UC_MACRO_ ## name); \
                wait-ms = <0>; \
                tap-ms = <0>; \
                #binding-cells = <0>; \
                bindings = <UC_LEAD_LINUX>, <&macro_tap unicode_bindings>, <UC_TRAIL_LINUX>; \
            }; \
        }; \
    };
#define UC_MACRO_MACWIN(name, unicode_bindings) \
    / { \
        macros { \
            name: name { \
                compatible = "zmk,behavior-macro"; \
                label = ZMK_HELPER_STRINGIFY(UC_MACRO_ ## name); \
                wait-ms = <0>; \
                tap-ms = <0>; \
                #binding-cells = <0>; \
                bindings = <UC_LEAD_MACWIN>, <&macro_tap unicode_bindings>, <UC_TRAIL_MACWIN>; \
            }; \
        }; \
    };

#define UC_MODMORPH(name, uc_binding, shifted_uc_binding) \
    / { \
        behaviors { \
            name: name { \
                compatible = "zmk,behavior-mod-morph"; \
                label = ZMK_HELPER_STRINGIFY(UC_MORPH_ ## name); \
                #binding-cells = <0>; \
                bindings = <uc_binding>, <shifted_uc_binding>; \
                mods = <(MOD_LSFT|MOD_RSFT)>; \
            }; \
        }; \
    };

#define ZMK_UNICODE_SINGLE(name, L0, L1, L2, L3) \
    UC_MACRO(name ## _lower, &kp L0 &kp L1 &kp L2 &kp L3) \
    UC_MODMORPH(name, &name ## _lower, &none)

#define ZMK_UNICODE_PAIR_LINUX(name, L0, L1, L2, L3, U0, U1, U2, U3) \
    UC_MACRO_LINUX(name ## _lower, &kp L0 &kp L1 &kp L2 &kp L3) \
    UC_MACRO_LINUX(name ## _upper, &kp U0 &kp U1 &kp U2 &kp U3) \
    UC_MODMORPH(name, &name ## _lower, &name ## _upper)

#define ZMK_UNICODE_PAIR_MACWIN(name, L0, L1, L2, L3, U0, U1, U2, U3) \
    UC_MACRO_MACWIN(name ## _lower, &kp L0 &kp L1 &kp L2 &kp L3) \
    UC_MACRO_MACWIN(name ## _upper, &kp U0 &kp U1 &kp U2 &kp U3) \
    UC_MODMORPH(name, &name ## _lower, &name ## _upper)

ZMK_UNICODE_PAIR_MACWIN(mwn_no_ae,  N0, N0,  E, N6,   N0, N0,  C, N6)
ZMK_UNICODE_PAIR_MACWIN(mwn_no_ao,  N0, N0,  E, N5,   N0, N0,  C, N5)
ZMK_UNICODE_PAIR_MACWIN(mwn_no_oe,  N0, N0,  F, N8,   N0, N0,  D, N8)
ZMK_UNICODE_PAIR_LINUX(lnx_no_ae,  N0, N0,  E, N6,   N0, N0,  C, N6)
ZMK_UNICODE_PAIR_LINUX(lnx_no_ao,  N0, N0,  E, N5,   N0, N0,  C, N5)
ZMK_UNICODE_PAIR_LINUX(lnx_no_oe,  N0, N0,  F, N8,   N0, N0,  D, N8)

