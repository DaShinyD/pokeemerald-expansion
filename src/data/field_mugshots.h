static const u32 sFieldMugshotGfx_TestNormal[] = INCBIN_U32("graphics/field_mugshots/test/normal.4bpp.lz");
static const u32 sFieldMugshotGfx_TestAlt[] = INCBIN_U32("graphics/field_mugshots/test/alt.4bpp.lz");
static const u16 sFieldMugshotPal_TestNormal[] = INCBIN_U16("graphics/field_mugshots/test/normal.gbapal");
static const u16 sFieldMugshotPal_TestAlt[] = INCBIN_U16("graphics/field_mugshots/test/alt.gbapal");
static const u32 sFieldMugshotGfx_Kemo[] = INCBIN_U32("graphics/field_mugshots/kemo/kemo.4bpp.lz");
static const u16 sFieldMugshotPal_Kemo[] = INCBIN_U16("graphics/field_mugshots/kemo/kemo.gbapal");
static const u32 sFieldMugshotGfx_Goku[] = INCBIN_U32("graphics/field_mugshots/goku/goku.4bpp.lz");
static const u16 sFieldMugshotPal_Goku[] = INCBIN_U16("graphics/field_mugshots/goku/goku.gbapal");
static const u32 sFieldMugshotGfx_Mcqueen[] = INCBIN_U32("graphics/field_mugshots/mcqueen/mcqueen.4bpp.lz");
static const u16 sFieldMugshotPal_Mcqueen[] = INCBIN_U16("graphics/field_mugshots/mcqueen/mcqueen.gbapal");
static const u32 sFieldMugshotGfx_Floyd[] = INCBIN_U32("graphics/field_mugshots/floyd/floyd.4bpp.lz");
static const u16 sFieldMugshotPal_Floyd[] = INCBIN_U16("graphics/field_mugshots/floyd/floyd.gbapal");
static const u32 sFieldMugshotGfx_Chauvin[] = INCBIN_U32("graphics/field_mugshots/chauvin/chauvin.4bpp.lz");
static const u16 sFieldMugshotPal_Chauvin[] = INCBIN_U16("graphics/field_mugshots/chauvin/chauvin.gbapal");
static const u32 sFieldMugshotGfx_Yugi[] = INCBIN_U32("graphics/field_mugshots/yugi/yugi.4bpp.lz");
static const u16 sFieldMugshotPal_Yugi[] = INCBIN_U16("graphics/field_mugshots/yugi/yugi.gbapal");
static const u32 sFieldMugshotGfx_Scary[] = INCBIN_U32("graphics/field_mugshots/scary/scary.4bpp.lz");
static const u16 sFieldMugshotPal_Scary[] = INCBIN_U16("graphics/field_mugshots/scary/scary.gbapal");
static const u32 sFieldMugshotGfx_Prescary[] = INCBIN_U32("graphics/field_mugshots/scary/prescary.4bpp.lz");
static const u16 sFieldMugshotPal_Prescary[] = INCBIN_U16("graphics/field_mugshots/scary/prescary.gbapal");

struct MugshotGfx
{
    const u32 *gfx;
    const u16 *pal;
};

static const struct MugshotGfx sFieldMugshots[MUGSHOT_COUNT][EMOTE_COUNT] =
{
    [MUGSHOT_TEST] =
    {
        [EMOTE_NORMAL] =
        {
            .gfx = sFieldMugshotGfx_TestNormal,
            .pal = sFieldMugshotPal_TestNormal,
        },

        [EMOTE_ALT] =
        {
            .gfx = sFieldMugshotGfx_TestAlt,
            .pal = sFieldMugshotPal_TestAlt,
        },
    },
    [MUGSHOT_KEMO] =
    {
        [EMOTE_NORMAL] =
        {
            .gfx = sFieldMugshotGfx_Kemo,
            .pal = sFieldMugshotPal_Kemo,
        },

        [EMOTE_ALT] =
        {
            .gfx = sFieldMugshotGfx_Kemo,
            .pal = sFieldMugshotPal_Kemo,
        },
    },
    [MUGSHOT_GOKU] =
    {
        [EMOTE_NORMAL] =
        {
            .gfx = sFieldMugshotGfx_Goku,
            .pal = sFieldMugshotPal_Goku,
        },

        [EMOTE_ALT] =
        {
            .gfx = sFieldMugshotGfx_Goku,
            .pal = sFieldMugshotPal_Goku,
        },
    },
    [MUGSHOT_MCQUEEN] =
    {
        [EMOTE_NORMAL] =
        {
            .gfx = sFieldMugshotGfx_Mcqueen,
            .pal = sFieldMugshotPal_Mcqueen,
        },

        [EMOTE_ALT] =
        {
            .gfx = sFieldMugshotGfx_Mcqueen,
            .pal = sFieldMugshotPal_Mcqueen,
        },
    },
    [MUGSHOT_CHAUVIN] =
    {
        [EMOTE_NORMAL] =
        {
            .gfx = sFieldMugshotGfx_Chauvin,
            .pal = sFieldMugshotPal_Chauvin,
        },

        [EMOTE_ALT] =
        {
            .gfx = sFieldMugshotGfx_Chauvin,
            .pal = sFieldMugshotPal_Chauvin,
        },
    },
    [MUGSHOT_FLOYD] =
    {
        [EMOTE_NORMAL] =
        {
            .gfx = sFieldMugshotGfx_Floyd,
            .pal = sFieldMugshotPal_Floyd,
        },

        [EMOTE_ALT] =
        {
            .gfx = sFieldMugshotGfx_Floyd,
            .pal = sFieldMugshotPal_Floyd,
        },
    },
    [MUGSHOT_YUGI] =
    {
        [EMOTE_NORMAL] =
        {
            .gfx = sFieldMugshotGfx_Yugi,
            .pal = sFieldMugshotPal_Yugi,
        },

        [EMOTE_ALT] =
        {
            .gfx = sFieldMugshotGfx_Yugi,
            .pal = sFieldMugshotPal_Yugi,
        },
    },
    [MUGSHOT_SCARY] =
    {
        [EMOTE_NORMAL] =
        {
            .gfx = sFieldMugshotGfx_Scary,
            .pal = sFieldMugshotPal_Scary,
        },

        [EMOTE_ALT] =
        {
            .gfx = sFieldMugshotGfx_Scary,
            .pal = sFieldMugshotPal_Scary,
        },
    },
    [MUGSHOT_PRE_SCARY] =
    {
        [EMOTE_NORMAL] =
        {
            .gfx = sFieldMugshotGfx_Prescary,
            .pal = sFieldMugshotPal_Prescary,
        },

        [EMOTE_ALT] =
        {
            .gfx = sFieldMugshotGfx_Prescary,
            .pal = sFieldMugshotPal_Prescary,
        },
    },
};
