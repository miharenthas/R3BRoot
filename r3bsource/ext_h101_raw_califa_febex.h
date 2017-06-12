/********************************************************
 *
 * Structure for ext_data_fetch_event() filling.
 *
 * Do not edit - automatically generated.
 */

#ifndef __GUARD_H101_EXT_H101_RAW_CALIFA_FEBEX_H__
#define __GUARD_H101_EXT_H101_RAW_CALIFA_FEBEX_H__

#ifndef __CINT__
# include <stdint.h>
#else
/* For CINT (old version trouble with stdint.h): */
# ifndef uint32_t
typedef unsigned int uint32_t;
typedef          int  int32_t;
# endif
#endif
#ifndef EXT_STRUCT_CTRL
# define EXT_STRUCT_CTRL(x)
#endif

/********************************************************
 *
 * Plain structure (layout as ntuple/root file):
 */

typedef struct EXT_STR_h101_CALIFA_t
{
  /* RAW */
  uint32_t CALIFA_ENE /* [0,144] */;
  uint32_t CALIFA_ENEI[144 EXT_STRUCT_CTRL(CALIFA_ENE)] /* [1,144] */;
  uint32_t CALIFA_ENEv[144 EXT_STRUCT_CTRL(CALIFA_ENE)] /* [0,65535] */;
  uint32_t CALIFA_TSLSB /* [0,144] */;
  uint32_t CALIFA_TSLSBI[144 EXT_STRUCT_CTRL(CALIFA_TSLSB)] /* [1,144] */;
  uint32_t CALIFA_TSLSBv[144 EXT_STRUCT_CTRL(CALIFA_TSLSB)] /* [-1,-1] */;
  uint32_t CALIFA_TSMSB /* [0,144] */;
  uint32_t CALIFA_TSMSBI[144 EXT_STRUCT_CTRL(CALIFA_TSMSB)] /* [1,144] */;
  uint32_t CALIFA_TSMSBv[144 EXT_STRUCT_CTRL(CALIFA_TSMSB)] /* [-1,-1] */;
  uint32_t CALIFA_NF /* [0,144] */;
  uint32_t CALIFA_NFI[144 EXT_STRUCT_CTRL(CALIFA_NF)] /* [1,144] */;
  uint32_t CALIFA_NFv[144 EXT_STRUCT_CTRL(CALIFA_NF)] /* [0,65535] */;
  uint32_t CALIFA_NS /* [0,144] */;
  uint32_t CALIFA_NSI[144 EXT_STRUCT_CTRL(CALIFA_NS)] /* [1,144] */;
  uint32_t CALIFA_NSv[144 EXT_STRUCT_CTRL(CALIFA_NS)] /* [0,65535] */;

} EXT_STR_h101_CALIFA;

/********************************************************
 *
 * Structure with multiple levels of arrays (partially)
 * recovered (recommended):
 */

typedef struct EXT_STR_h101_CALIFA_onion_t
{
  /* RAW */
  uint32_t CALIFA_ENE;
  uint32_t CALIFA_ENEI[144 /* CALIFA_ENE */];
  uint32_t CALIFA_ENEv[144 /* CALIFA_ENE */];
  uint32_t CALIFA_TSLSB;
  uint32_t CALIFA_TSLSBI[144 /* CALIFA_TSLSB */];
  uint32_t CALIFA_TSLSBv[144 /* CALIFA_TSLSB */];
  uint32_t CALIFA_TSMSB;
  uint32_t CALIFA_TSMSBI[144 /* CALIFA_TSMSB */];
  uint32_t CALIFA_TSMSBv[144 /* CALIFA_TSMSB */];
  uint32_t CALIFA_NF;
  uint32_t CALIFA_NFI[144 /* CALIFA_NF */];
  uint32_t CALIFA_NFv[144 /* CALIFA_NF */];
  uint32_t CALIFA_NS;
  uint32_t CALIFA_NSI[144 /* CALIFA_NS */];
  uint32_t CALIFA_NSv[144 /* CALIFA_NS */];

} EXT_STR_h101_CALIFA_onion;

/*******************************************************/

#define EXT_STR_h101_CALIFA_ITEMS_INFO(ok,si,offset,struct_t,printerr) do { \
  ok = 1; \
  /* RAW */ \
  EXT_STR_ITEM_INFO_LIM(ok,si,offset,struct_t,printerr,\
                     CALIFA_ENE,                      UINT32,\
                    "CALIFA_ENE",144); \
  EXT_STR_ITEM_INFO_ZZP(ok,si,offset,struct_t,printerr,\
                     CALIFA_ENEI,                     UINT32,\
                    "CALIFA_ENEI",                     "CALIFA_ENE"); \
  EXT_STR_ITEM_INFO_ZZP(ok,si,offset,struct_t,printerr,\
                     CALIFA_ENEv,                     UINT32,\
                    "CALIFA_ENEv",                     "CALIFA_ENE"); \
  EXT_STR_ITEM_INFO_LIM(ok,si,offset,struct_t,printerr,\
                     CALIFA_TSLSB,                    UINT32,\
                    "CALIFA_TSLSB",144); \
  EXT_STR_ITEM_INFO_ZZP(ok,si,offset,struct_t,printerr,\
                     CALIFA_TSLSBI,                   UINT32,\
                    "CALIFA_TSLSBI",                   "CALIFA_TSLSB"); \
  EXT_STR_ITEM_INFO_ZZP(ok,si,offset,struct_t,printerr,\
                     CALIFA_TSLSBv,                   UINT32,\
                    "CALIFA_TSLSBv",                   "CALIFA_TSLSB"); \
  EXT_STR_ITEM_INFO_LIM(ok,si,offset,struct_t,printerr,\
                     CALIFA_TSMSB,                    UINT32,\
                    "CALIFA_TSMSB",144); \
  EXT_STR_ITEM_INFO_ZZP(ok,si,offset,struct_t,printerr,\
                     CALIFA_TSMSBI,                   UINT32,\
                    "CALIFA_TSMSBI",                   "CALIFA_TSMSB"); \
  EXT_STR_ITEM_INFO_ZZP(ok,si,offset,struct_t,printerr,\
                     CALIFA_TSMSBv,                   UINT32,\
                    "CALIFA_TSMSBv",                   "CALIFA_TSMSB"); \
  EXT_STR_ITEM_INFO_LIM(ok,si,offset,struct_t,printerr,\
                     CALIFA_NF,                       UINT32,\
                    "CALIFA_NF",144); \
  EXT_STR_ITEM_INFO_ZZP(ok,si,offset,struct_t,printerr,\
                     CALIFA_NFI,                      UINT32,\
                    "CALIFA_NFI",                      "CALIFA_NF"); \
  EXT_STR_ITEM_INFO_ZZP(ok,si,offset,struct_t,printerr,\
                     CALIFA_NFv,                      UINT32,\
                    "CALIFA_NFv",                      "CALIFA_NF"); \
  EXT_STR_ITEM_INFO_LIM(ok,si,offset,struct_t,printerr,\
                     CALIFA_NS,                       UINT32,\
                    "CALIFA_NS",144); \
  EXT_STR_ITEM_INFO_ZZP(ok,si,offset,struct_t,printerr,\
                     CALIFA_NSI,                      UINT32,\
                    "CALIFA_NSI",                      "CALIFA_NS"); \
  EXT_STR_ITEM_INFO_ZZP(ok,si,offset,struct_t,printerr,\
                     CALIFA_NSv,                      UINT32,\
                    "CALIFA_NSv",                      "CALIFA_NS"); \
  \
} while (0);

/********************************************************
 *
 * For internal use by the network data reader:
 * (version checks, etc)
 */

typedef struct EXT_STR_h101_CALIFA_layout_t
{
  uint32_t _magic;
  uint32_t _size_info;
  uint32_t _size_struct;
  uint32_t _size_struct_onion;
  uint32_t _pack_list_items;

  uint32_t _num_items;
  struct {
    uint32_t _offset;
    uint32_t _size;
    uint32_t _xor;
    const char *_name;
  } _items[1];
  uint32_t _pack_list[1455];
} EXT_STR_h101_CALIFA_layout;

#define EXT_STR_h101_CALIFA_LAYOUT_INIT { \
  0x57e65c93, \
  sizeof(EXT_STR_h101_CALIFA_layout), \
  sizeof(EXT_STR_h101_CALIFA), \
  sizeof(EXT_STR_h101_CALIFA_onion), \
  1455, \
  1, \
  { \
    { 0, sizeof(EXT_STR_h101_CALIFA), 0x4d221b8e, "h101" }, \
  }, \
  { \
    0xc0000000, 0x00000090, 0x00000002, 0x40000004, \
    0x40000244, 0x40000008, 0x40000248, 0x4000000c, \
    0x4000024c, 0x40000010, 0x40000250, 0x40000014, \
    0x40000254, 0x40000018, 0x40000258, 0x4000001c, \
    0x4000025c, 0x40000020, 0x40000260, 0x40000024, \
    0x40000264, 0x40000028, 0x40000268, 0x4000002c, \
    0x4000026c, 0x40000030, 0x40000270, 0x40000034, \
    0x40000274, 0x40000038, 0x40000278, 0x4000003c, \
    0x4000027c, 0x40000040, 0x40000280, 0x40000044, \
    0x40000284, 0x40000048, 0x40000288, 0x4000004c, \
    0x4000028c, 0x40000050, 0x40000290, 0x40000054, \
    0x40000294, 0x40000058, 0x40000298, 0x4000005c, \
    0x4000029c, 0x40000060, 0x400002a0, 0x40000064, \
    0x400002a4, 0x40000068, 0x400002a8, 0x4000006c, \
    0x400002ac, 0x40000070, 0x400002b0, 0x40000074, \
    0x400002b4, 0x40000078, 0x400002b8, 0x4000007c, \
    0x400002bc, 0x40000080, 0x400002c0, 0x40000084, \
    0x400002c4, 0x40000088, 0x400002c8, 0x4000008c, \
    0x400002cc, 0x40000090, 0x400002d0, 0x40000094, \
    0x400002d4, 0x40000098, 0x400002d8, 0x4000009c, \
    0x400002dc, 0x400000a0, 0x400002e0, 0x400000a4, \
    0x400002e4, 0x400000a8, 0x400002e8, 0x400000ac, \
    0x400002ec, 0x400000b0, 0x400002f0, 0x400000b4, \
    0x400002f4, 0x400000b8, 0x400002f8, 0x400000bc, \
    0x400002fc, 0x400000c0, 0x40000300, 0x400000c4, \
    0x40000304, 0x400000c8, 0x40000308, 0x400000cc, \
    0x4000030c, 0x400000d0, 0x40000310, 0x400000d4, \
    0x40000314, 0x400000d8, 0x40000318, 0x400000dc, \
    0x4000031c, 0x400000e0, 0x40000320, 0x400000e4, \
    0x40000324, 0x400000e8, 0x40000328, 0x400000ec, \
    0x4000032c, 0x400000f0, 0x40000330, 0x400000f4, \
    0x40000334, 0x400000f8, 0x40000338, 0x400000fc, \
    0x4000033c, 0x40000100, 0x40000340, 0x40000104, \
    0x40000344, 0x40000108, 0x40000348, 0x4000010c, \
    0x4000034c, 0x40000110, 0x40000350, 0x40000114, \
    0x40000354, 0x40000118, 0x40000358, 0x4000011c, \
    0x4000035c, 0x40000120, 0x40000360, 0x40000124, \
    0x40000364, 0x40000128, 0x40000368, 0x4000012c, \
    0x4000036c, 0x40000130, 0x40000370, 0x40000134, \
    0x40000374, 0x40000138, 0x40000378, 0x4000013c, \
    0x4000037c, 0x40000140, 0x40000380, 0x40000144, \
    0x40000384, 0x40000148, 0x40000388, 0x4000014c, \
    0x4000038c, 0x40000150, 0x40000390, 0x40000154, \
    0x40000394, 0x40000158, 0x40000398, 0x4000015c, \
    0x4000039c, 0x40000160, 0x400003a0, 0x40000164, \
    0x400003a4, 0x40000168, 0x400003a8, 0x4000016c, \
    0x400003ac, 0x40000170, 0x400003b0, 0x40000174, \
    0x400003b4, 0x40000178, 0x400003b8, 0x4000017c, \
    0x400003bc, 0x40000180, 0x400003c0, 0x40000184, \
    0x400003c4, 0x40000188, 0x400003c8, 0x4000018c, \
    0x400003cc, 0x40000190, 0x400003d0, 0x40000194, \
    0x400003d4, 0x40000198, 0x400003d8, 0x4000019c, \
    0x400003dc, 0x400001a0, 0x400003e0, 0x400001a4, \
    0x400003e4, 0x400001a8, 0x400003e8, 0x400001ac, \
    0x400003ec, 0x400001b0, 0x400003f0, 0x400001b4, \
    0x400003f4, 0x400001b8, 0x400003f8, 0x400001bc, \
    0x400003fc, 0x400001c0, 0x40000400, 0x400001c4, \
    0x40000404, 0x400001c8, 0x40000408, 0x400001cc, \
    0x4000040c, 0x400001d0, 0x40000410, 0x400001d4, \
    0x40000414, 0x400001d8, 0x40000418, 0x400001dc, \
    0x4000041c, 0x400001e0, 0x40000420, 0x400001e4, \
    0x40000424, 0x400001e8, 0x40000428, 0x400001ec, \
    0x4000042c, 0x400001f0, 0x40000430, 0x400001f4, \
    0x40000434, 0x400001f8, 0x40000438, 0x400001fc, \
    0x4000043c, 0x40000200, 0x40000440, 0x40000204, \
    0x40000444, 0x40000208, 0x40000448, 0x4000020c, \
    0x4000044c, 0x40000210, 0x40000450, 0x40000214, \
    0x40000454, 0x40000218, 0x40000458, 0x4000021c, \
    0x4000045c, 0x40000220, 0x40000460, 0x40000224, \
    0x40000464, 0x40000228, 0x40000468, 0x4000022c, \
    0x4000046c, 0x40000230, 0x40000470, 0x40000234, \
    0x40000474, 0x40000238, 0x40000478, 0x4000023c, \
    0x4000047c, 0x40000240, 0x40000480, 0xc0000484, \
    0x00000090, 0x00000002, 0x40000488, 0x400006c8, \
    0x4000048c, 0x400006cc, 0x40000490, 0x400006d0, \
    0x40000494, 0x400006d4, 0x40000498, 0x400006d8, \
    0x4000049c, 0x400006dc, 0x400004a0, 0x400006e0, \
    0x400004a4, 0x400006e4, 0x400004a8, 0x400006e8, \
    0x400004ac, 0x400006ec, 0x400004b0, 0x400006f0, \
    0x400004b4, 0x400006f4, 0x400004b8, 0x400006f8, \
    0x400004bc, 0x400006fc, 0x400004c0, 0x40000700, \
    0x400004c4, 0x40000704, 0x400004c8, 0x40000708, \
    0x400004cc, 0x4000070c, 0x400004d0, 0x40000710, \
    0x400004d4, 0x40000714, 0x400004d8, 0x40000718, \
    0x400004dc, 0x4000071c, 0x400004e0, 0x40000720, \
    0x400004e4, 0x40000724, 0x400004e8, 0x40000728, \
    0x400004ec, 0x4000072c, 0x400004f0, 0x40000730, \
    0x400004f4, 0x40000734, 0x400004f8, 0x40000738, \
    0x400004fc, 0x4000073c, 0x40000500, 0x40000740, \
    0x40000504, 0x40000744, 0x40000508, 0x40000748, \
    0x4000050c, 0x4000074c, 0x40000510, 0x40000750, \
    0x40000514, 0x40000754, 0x40000518, 0x40000758, \
    0x4000051c, 0x4000075c, 0x40000520, 0x40000760, \
    0x40000524, 0x40000764, 0x40000528, 0x40000768, \
    0x4000052c, 0x4000076c, 0x40000530, 0x40000770, \
    0x40000534, 0x40000774, 0x40000538, 0x40000778, \
    0x4000053c, 0x4000077c, 0x40000540, 0x40000780, \
    0x40000544, 0x40000784, 0x40000548, 0x40000788, \
    0x4000054c, 0x4000078c, 0x40000550, 0x40000790, \
    0x40000554, 0x40000794, 0x40000558, 0x40000798, \
    0x4000055c, 0x4000079c, 0x40000560, 0x400007a0, \
    0x40000564, 0x400007a4, 0x40000568, 0x400007a8, \
    0x4000056c, 0x400007ac, 0x40000570, 0x400007b0, \
    0x40000574, 0x400007b4, 0x40000578, 0x400007b8, \
    0x4000057c, 0x400007bc, 0x40000580, 0x400007c0, \
    0x40000584, 0x400007c4, 0x40000588, 0x400007c8, \
    0x4000058c, 0x400007cc, 0x40000590, 0x400007d0, \
    0x40000594, 0x400007d4, 0x40000598, 0x400007d8, \
    0x4000059c, 0x400007dc, 0x400005a0, 0x400007e0, \
    0x400005a4, 0x400007e4, 0x400005a8, 0x400007e8, \
    0x400005ac, 0x400007ec, 0x400005b0, 0x400007f0, \
    0x400005b4, 0x400007f4, 0x400005b8, 0x400007f8, \
    0x400005bc, 0x400007fc, 0x400005c0, 0x40000800, \
    0x400005c4, 0x40000804, 0x400005c8, 0x40000808, \
    0x400005cc, 0x4000080c, 0x400005d0, 0x40000810, \
    0x400005d4, 0x40000814, 0x400005d8, 0x40000818, \
    0x400005dc, 0x4000081c, 0x400005e0, 0x40000820, \
    0x400005e4, 0x40000824, 0x400005e8, 0x40000828, \
    0x400005ec, 0x4000082c, 0x400005f0, 0x40000830, \
    0x400005f4, 0x40000834, 0x400005f8, 0x40000838, \
    0x400005fc, 0x4000083c, 0x40000600, 0x40000840, \
    0x40000604, 0x40000844, 0x40000608, 0x40000848, \
    0x4000060c, 0x4000084c, 0x40000610, 0x40000850, \
    0x40000614, 0x40000854, 0x40000618, 0x40000858, \
    0x4000061c, 0x4000085c, 0x40000620, 0x40000860, \
    0x40000624, 0x40000864, 0x40000628, 0x40000868, \
    0x4000062c, 0x4000086c, 0x40000630, 0x40000870, \
    0x40000634, 0x40000874, 0x40000638, 0x40000878, \
    0x4000063c, 0x4000087c, 0x40000640, 0x40000880, \
    0x40000644, 0x40000884, 0x40000648, 0x40000888, \
    0x4000064c, 0x4000088c, 0x40000650, 0x40000890, \
    0x40000654, 0x40000894, 0x40000658, 0x40000898, \
    0x4000065c, 0x4000089c, 0x40000660, 0x400008a0, \
    0x40000664, 0x400008a4, 0x40000668, 0x400008a8, \
    0x4000066c, 0x400008ac, 0x40000670, 0x400008b0, \
    0x40000674, 0x400008b4, 0x40000678, 0x400008b8, \
    0x4000067c, 0x400008bc, 0x40000680, 0x400008c0, \
    0x40000684, 0x400008c4, 0x40000688, 0x400008c8, \
    0x4000068c, 0x400008cc, 0x40000690, 0x400008d0, \
    0x40000694, 0x400008d4, 0x40000698, 0x400008d8, \
    0x4000069c, 0x400008dc, 0x400006a0, 0x400008e0, \
    0x400006a4, 0x400008e4, 0x400006a8, 0x400008e8, \
    0x400006ac, 0x400008ec, 0x400006b0, 0x400008f0, \
    0x400006b4, 0x400008f4, 0x400006b8, 0x400008f8, \
    0x400006bc, 0x400008fc, 0x400006c0, 0x40000900, \
    0x400006c4, 0x40000904, 0xc0000908, 0x00000090, \
    0x00000002, 0x4000090c, 0x40000b4c, 0x40000910, \
    0x40000b50, 0x40000914, 0x40000b54, 0x40000918, \
    0x40000b58, 0x4000091c, 0x40000b5c, 0x40000920, \
    0x40000b60, 0x40000924, 0x40000b64, 0x40000928, \
    0x40000b68, 0x4000092c, 0x40000b6c, 0x40000930, \
    0x40000b70, 0x40000934, 0x40000b74, 0x40000938, \
    0x40000b78, 0x4000093c, 0x40000b7c, 0x40000940, \
    0x40000b80, 0x40000944, 0x40000b84, 0x40000948, \
    0x40000b88, 0x4000094c, 0x40000b8c, 0x40000950, \
    0x40000b90, 0x40000954, 0x40000b94, 0x40000958, \
    0x40000b98, 0x4000095c, 0x40000b9c, 0x40000960, \
    0x40000ba0, 0x40000964, 0x40000ba4, 0x40000968, \
    0x40000ba8, 0x4000096c, 0x40000bac, 0x40000970, \
    0x40000bb0, 0x40000974, 0x40000bb4, 0x40000978, \
    0x40000bb8, 0x4000097c, 0x40000bbc, 0x40000980, \
    0x40000bc0, 0x40000984, 0x40000bc4, 0x40000988, \
    0x40000bc8, 0x4000098c, 0x40000bcc, 0x40000990, \
    0x40000bd0, 0x40000994, 0x40000bd4, 0x40000998, \
    0x40000bd8, 0x4000099c, 0x40000bdc, 0x400009a0, \
    0x40000be0, 0x400009a4, 0x40000be4, 0x400009a8, \
    0x40000be8, 0x400009ac, 0x40000bec, 0x400009b0, \
    0x40000bf0, 0x400009b4, 0x40000bf4, 0x400009b8, \
    0x40000bf8, 0x400009bc, 0x40000bfc, 0x400009c0, \
    0x40000c00, 0x400009c4, 0x40000c04, 0x400009c8, \
    0x40000c08, 0x400009cc, 0x40000c0c, 0x400009d0, \
    0x40000c10, 0x400009d4, 0x40000c14, 0x400009d8, \
    0x40000c18, 0x400009dc, 0x40000c1c, 0x400009e0, \
    0x40000c20, 0x400009e4, 0x40000c24, 0x400009e8, \
    0x40000c28, 0x400009ec, 0x40000c2c, 0x400009f0, \
    0x40000c30, 0x400009f4, 0x40000c34, 0x400009f8, \
    0x40000c38, 0x400009fc, 0x40000c3c, 0x40000a00, \
    0x40000c40, 0x40000a04, 0x40000c44, 0x40000a08, \
    0x40000c48, 0x40000a0c, 0x40000c4c, 0x40000a10, \
    0x40000c50, 0x40000a14, 0x40000c54, 0x40000a18, \
    0x40000c58, 0x40000a1c, 0x40000c5c, 0x40000a20, \
    0x40000c60, 0x40000a24, 0x40000c64, 0x40000a28, \
    0x40000c68, 0x40000a2c, 0x40000c6c, 0x40000a30, \
    0x40000c70, 0x40000a34, 0x40000c74, 0x40000a38, \
    0x40000c78, 0x40000a3c, 0x40000c7c, 0x40000a40, \
    0x40000c80, 0x40000a44, 0x40000c84, 0x40000a48, \
    0x40000c88, 0x40000a4c, 0x40000c8c, 0x40000a50, \
    0x40000c90, 0x40000a54, 0x40000c94, 0x40000a58, \
    0x40000c98, 0x40000a5c, 0x40000c9c, 0x40000a60, \
    0x40000ca0, 0x40000a64, 0x40000ca4, 0x40000a68, \
    0x40000ca8, 0x40000a6c, 0x40000cac, 0x40000a70, \
    0x40000cb0, 0x40000a74, 0x40000cb4, 0x40000a78, \
    0x40000cb8, 0x40000a7c, 0x40000cbc, 0x40000a80, \
    0x40000cc0, 0x40000a84, 0x40000cc4, 0x40000a88, \
    0x40000cc8, 0x40000a8c, 0x40000ccc, 0x40000a90, \
    0x40000cd0, 0x40000a94, 0x40000cd4, 0x40000a98, \
    0x40000cd8, 0x40000a9c, 0x40000cdc, 0x40000aa0, \
    0x40000ce0, 0x40000aa4, 0x40000ce4, 0x40000aa8, \
    0x40000ce8, 0x40000aac, 0x40000cec, 0x40000ab0, \
    0x40000cf0, 0x40000ab4, 0x40000cf4, 0x40000ab8, \
    0x40000cf8, 0x40000abc, 0x40000cfc, 0x40000ac0, \
    0x40000d00, 0x40000ac4, 0x40000d04, 0x40000ac8, \
    0x40000d08, 0x40000acc, 0x40000d0c, 0x40000ad0, \
    0x40000d10, 0x40000ad4, 0x40000d14, 0x40000ad8, \
    0x40000d18, 0x40000adc, 0x40000d1c, 0x40000ae0, \
    0x40000d20, 0x40000ae4, 0x40000d24, 0x40000ae8, \
    0x40000d28, 0x40000aec, 0x40000d2c, 0x40000af0, \
    0x40000d30, 0x40000af4, 0x40000d34, 0x40000af8, \
    0x40000d38, 0x40000afc, 0x40000d3c, 0x40000b00, \
    0x40000d40, 0x40000b04, 0x40000d44, 0x40000b08, \
    0x40000d48, 0x40000b0c, 0x40000d4c, 0x40000b10, \
    0x40000d50, 0x40000b14, 0x40000d54, 0x40000b18, \
    0x40000d58, 0x40000b1c, 0x40000d5c, 0x40000b20, \
    0x40000d60, 0x40000b24, 0x40000d64, 0x40000b28, \
    0x40000d68, 0x40000b2c, 0x40000d6c, 0x40000b30, \
    0x40000d70, 0x40000b34, 0x40000d74, 0x40000b38, \
    0x40000d78, 0x40000b3c, 0x40000d7c, 0x40000b40, \
    0x40000d80, 0x40000b44, 0x40000d84, 0x40000b48, \
    0x40000d88, 0xc0000d8c, 0x00000090, 0x00000002, \
    0x40000d90, 0x40000fd0, 0x40000d94, 0x40000fd4, \
    0x40000d98, 0x40000fd8, 0x40000d9c, 0x40000fdc, \
    0x40000da0, 0x40000fe0, 0x40000da4, 0x40000fe4, \
    0x40000da8, 0x40000fe8, 0x40000dac, 0x40000fec, \
    0x40000db0, 0x40000ff0, 0x40000db4, 0x40000ff4, \
    0x40000db8, 0x40000ff8, 0x40000dbc, 0x40000ffc, \
    0x40000dc0, 0x40001000, 0x40000dc4, 0x40001004, \
    0x40000dc8, 0x40001008, 0x40000dcc, 0x4000100c, \
    0x40000dd0, 0x40001010, 0x40000dd4, 0x40001014, \
    0x40000dd8, 0x40001018, 0x40000ddc, 0x4000101c, \
    0x40000de0, 0x40001020, 0x40000de4, 0x40001024, \
    0x40000de8, 0x40001028, 0x40000dec, 0x4000102c, \
    0x40000df0, 0x40001030, 0x40000df4, 0x40001034, \
    0x40000df8, 0x40001038, 0x40000dfc, 0x4000103c, \
    0x40000e00, 0x40001040, 0x40000e04, 0x40001044, \
    0x40000e08, 0x40001048, 0x40000e0c, 0x4000104c, \
    0x40000e10, 0x40001050, 0x40000e14, 0x40001054, \
    0x40000e18, 0x40001058, 0x40000e1c, 0x4000105c, \
    0x40000e20, 0x40001060, 0x40000e24, 0x40001064, \
    0x40000e28, 0x40001068, 0x40000e2c, 0x4000106c, \
    0x40000e30, 0x40001070, 0x40000e34, 0x40001074, \
    0x40000e38, 0x40001078, 0x40000e3c, 0x4000107c, \
    0x40000e40, 0x40001080, 0x40000e44, 0x40001084, \
    0x40000e48, 0x40001088, 0x40000e4c, 0x4000108c, \
    0x40000e50, 0x40001090, 0x40000e54, 0x40001094, \
    0x40000e58, 0x40001098, 0x40000e5c, 0x4000109c, \
    0x40000e60, 0x400010a0, 0x40000e64, 0x400010a4, \
    0x40000e68, 0x400010a8, 0x40000e6c, 0x400010ac, \
    0x40000e70, 0x400010b0, 0x40000e74, 0x400010b4, \
    0x40000e78, 0x400010b8, 0x40000e7c, 0x400010bc, \
    0x40000e80, 0x400010c0, 0x40000e84, 0x400010c4, \
    0x40000e88, 0x400010c8, 0x40000e8c, 0x400010cc, \
    0x40000e90, 0x400010d0, 0x40000e94, 0x400010d4, \
    0x40000e98, 0x400010d8, 0x40000e9c, 0x400010dc, \
    0x40000ea0, 0x400010e0, 0x40000ea4, 0x400010e4, \
    0x40000ea8, 0x400010e8, 0x40000eac, 0x400010ec, \
    0x40000eb0, 0x400010f0, 0x40000eb4, 0x400010f4, \
    0x40000eb8, 0x400010f8, 0x40000ebc, 0x400010fc, \
    0x40000ec0, 0x40001100, 0x40000ec4, 0x40001104, \
    0x40000ec8, 0x40001108, 0x40000ecc, 0x4000110c, \
    0x40000ed0, 0x40001110, 0x40000ed4, 0x40001114, \
    0x40000ed8, 0x40001118, 0x40000edc, 0x4000111c, \
    0x40000ee0, 0x40001120, 0x40000ee4, 0x40001124, \
    0x40000ee8, 0x40001128, 0x40000eec, 0x4000112c, \
    0x40000ef0, 0x40001130, 0x40000ef4, 0x40001134, \
    0x40000ef8, 0x40001138, 0x40000efc, 0x4000113c, \
    0x40000f00, 0x40001140, 0x40000f04, 0x40001144, \
    0x40000f08, 0x40001148, 0x40000f0c, 0x4000114c, \
    0x40000f10, 0x40001150, 0x40000f14, 0x40001154, \
    0x40000f18, 0x40001158, 0x40000f1c, 0x4000115c, \
    0x40000f20, 0x40001160, 0x40000f24, 0x40001164, \
    0x40000f28, 0x40001168, 0x40000f2c, 0x4000116c, \
    0x40000f30, 0x40001170, 0x40000f34, 0x40001174, \
    0x40000f38, 0x40001178, 0x40000f3c, 0x4000117c, \
    0x40000f40, 0x40001180, 0x40000f44, 0x40001184, \
    0x40000f48, 0x40001188, 0x40000f4c, 0x4000118c, \
    0x40000f50, 0x40001190, 0x40000f54, 0x40001194, \
    0x40000f58, 0x40001198, 0x40000f5c, 0x4000119c, \
    0x40000f60, 0x400011a0, 0x40000f64, 0x400011a4, \
    0x40000f68, 0x400011a8, 0x40000f6c, 0x400011ac, \
    0x40000f70, 0x400011b0, 0x40000f74, 0x400011b4, \
    0x40000f78, 0x400011b8, 0x40000f7c, 0x400011bc, \
    0x40000f80, 0x400011c0, 0x40000f84, 0x400011c4, \
    0x40000f88, 0x400011c8, 0x40000f8c, 0x400011cc, \
    0x40000f90, 0x400011d0, 0x40000f94, 0x400011d4, \
    0x40000f98, 0x400011d8, 0x40000f9c, 0x400011dc, \
    0x40000fa0, 0x400011e0, 0x40000fa4, 0x400011e4, \
    0x40000fa8, 0x400011e8, 0x40000fac, 0x400011ec, \
    0x40000fb0, 0x400011f0, 0x40000fb4, 0x400011f4, \
    0x40000fb8, 0x400011f8, 0x40000fbc, 0x400011fc, \
    0x40000fc0, 0x40001200, 0x40000fc4, 0x40001204, \
    0x40000fc8, 0x40001208, 0x40000fcc, 0x4000120c, \
    0xc0001210, 0x00000090, 0x00000002, 0x40001214, \
    0x40001454, 0x40001218, 0x40001458, 0x4000121c, \
    0x4000145c, 0x40001220, 0x40001460, 0x40001224, \
    0x40001464, 0x40001228, 0x40001468, 0x4000122c, \
    0x4000146c, 0x40001230, 0x40001470, 0x40001234, \
    0x40001474, 0x40001238, 0x40001478, 0x4000123c, \
    0x4000147c, 0x40001240, 0x40001480, 0x40001244, \
    0x40001484, 0x40001248, 0x40001488, 0x4000124c, \
    0x4000148c, 0x40001250, 0x40001490, 0x40001254, \
    0x40001494, 0x40001258, 0x40001498, 0x4000125c, \
    0x4000149c, 0x40001260, 0x400014a0, 0x40001264, \
    0x400014a4, 0x40001268, 0x400014a8, 0x4000126c, \
    0x400014ac, 0x40001270, 0x400014b0, 0x40001274, \
    0x400014b4, 0x40001278, 0x400014b8, 0x4000127c, \
    0x400014bc, 0x40001280, 0x400014c0, 0x40001284, \
    0x400014c4, 0x40001288, 0x400014c8, 0x4000128c, \
    0x400014cc, 0x40001290, 0x400014d0, 0x40001294, \
    0x400014d4, 0x40001298, 0x400014d8, 0x4000129c, \
    0x400014dc, 0x400012a0, 0x400014e0, 0x400012a4, \
    0x400014e4, 0x400012a8, 0x400014e8, 0x400012ac, \
    0x400014ec, 0x400012b0, 0x400014f0, 0x400012b4, \
    0x400014f4, 0x400012b8, 0x400014f8, 0x400012bc, \
    0x400014fc, 0x400012c0, 0x40001500, 0x400012c4, \
    0x40001504, 0x400012c8, 0x40001508, 0x400012cc, \
    0x4000150c, 0x400012d0, 0x40001510, 0x400012d4, \
    0x40001514, 0x400012d8, 0x40001518, 0x400012dc, \
    0x4000151c, 0x400012e0, 0x40001520, 0x400012e4, \
    0x40001524, 0x400012e8, 0x40001528, 0x400012ec, \
    0x4000152c, 0x400012f0, 0x40001530, 0x400012f4, \
    0x40001534, 0x400012f8, 0x40001538, 0x400012fc, \
    0x4000153c, 0x40001300, 0x40001540, 0x40001304, \
    0x40001544, 0x40001308, 0x40001548, 0x4000130c, \
    0x4000154c, 0x40001310, 0x40001550, 0x40001314, \
    0x40001554, 0x40001318, 0x40001558, 0x4000131c, \
    0x4000155c, 0x40001320, 0x40001560, 0x40001324, \
    0x40001564, 0x40001328, 0x40001568, 0x4000132c, \
    0x4000156c, 0x40001330, 0x40001570, 0x40001334, \
    0x40001574, 0x40001338, 0x40001578, 0x4000133c, \
    0x4000157c, 0x40001340, 0x40001580, 0x40001344, \
    0x40001584, 0x40001348, 0x40001588, 0x4000134c, \
    0x4000158c, 0x40001350, 0x40001590, 0x40001354, \
    0x40001594, 0x40001358, 0x40001598, 0x4000135c, \
    0x4000159c, 0x40001360, 0x400015a0, 0x40001364, \
    0x400015a4, 0x40001368, 0x400015a8, 0x4000136c, \
    0x400015ac, 0x40001370, 0x400015b0, 0x40001374, \
    0x400015b4, 0x40001378, 0x400015b8, 0x4000137c, \
    0x400015bc, 0x40001380, 0x400015c0, 0x40001384, \
    0x400015c4, 0x40001388, 0x400015c8, 0x4000138c, \
    0x400015cc, 0x40001390, 0x400015d0, 0x40001394, \
    0x400015d4, 0x40001398, 0x400015d8, 0x4000139c, \
    0x400015dc, 0x400013a0, 0x400015e0, 0x400013a4, \
    0x400015e4, 0x400013a8, 0x400015e8, 0x400013ac, \
    0x400015ec, 0x400013b0, 0x400015f0, 0x400013b4, \
    0x400015f4, 0x400013b8, 0x400015f8, 0x400013bc, \
    0x400015fc, 0x400013c0, 0x40001600, 0x400013c4, \
    0x40001604, 0x400013c8, 0x40001608, 0x400013cc, \
    0x4000160c, 0x400013d0, 0x40001610, 0x400013d4, \
    0x40001614, 0x400013d8, 0x40001618, 0x400013dc, \
    0x4000161c, 0x400013e0, 0x40001620, 0x400013e4, \
    0x40001624, 0x400013e8, 0x40001628, 0x400013ec, \
    0x4000162c, 0x400013f0, 0x40001630, 0x400013f4, \
    0x40001634, 0x400013f8, 0x40001638, 0x400013fc, \
    0x4000163c, 0x40001400, 0x40001640, 0x40001404, \
    0x40001644, 0x40001408, 0x40001648, 0x4000140c, \
    0x4000164c, 0x40001410, 0x40001650, 0x40001414, \
    0x40001654, 0x40001418, 0x40001658, 0x4000141c, \
    0x4000165c, 0x40001420, 0x40001660, 0x40001424, \
    0x40001664, 0x40001428, 0x40001668, 0x4000142c, \
    0x4000166c, 0x40001430, 0x40001670, 0x40001434, \
    0x40001674, 0x40001438, 0x40001678, 0x4000143c, \
    0x4000167c, 0x40001440, 0x40001680, 0x40001444, \
    0x40001684, 0x40001448, 0x40001688, 0x4000144c, \
    0x4000168c, 0x40001450, 0x40001690, \
  } \
};

#endif/*__GUARD_H101_EXT_H101_RAW_CALIFA_FEBEX_H__*/

/*******************************************************/
