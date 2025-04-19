/* See LICENSE file for copyright and license details. */

/* Constants */
#define TERMINAL "st"
#define TERMCLASS "St"

/* appearance */
static unsigned int borderpx  = 3;        /* border pixel of windows */
static unsigned int snap      = 32;       /* snap pixel */
static unsigned int gappih    = 20;       /* horiz inner gap between windows */
static unsigned int gappiv    = 10;       /* vert inner gap between windows */
static unsigned int gappoh    = 10;       /* horiz outer gap between windows and screen edge */
static unsigned int gappov    = 30;       /* vert outer gap between windows and screen edge */
static int swallowfloating    = 0;        /* 1 means swallow floating windows by default */
static int smartgaps          = 0;        /* 1 means no outer gap when there is only one window */
static int showbar            = 1;        /* 0 means no bar */
static int topbar             = 1;        /* 0 means bottom bar */
static char *fonts[]          = { "FantasqueSansMono Nerd Font Mono Regular:size=10", "JoyPixels:pixelsize=10:antialias=true:autohint=true"  };
static char normbgcolor[]           = "#222222";
static char normbordercolor[]       = "#444444";
static char normfgcolor[]           = "#fffffb";
static char selfgcolor[]            = "#eeeeee";
/* static char selbordercolor[]        = "#770000"; */
static char selbordercolor[]        = "#45b97c";
/* static char selbgcolor[]            = "#005577"; */
static char selbgcolor[]            = "#37474F";
static const unsigned int baralpha = 0x64;
static const unsigned int borderalpha = OPAQUE;
static const char *colors[][3] = {
       /*               fg           bg           border   */
       [SchemeNorm] = { normfgcolor, normbgcolor, normbordercolor },
       [SchemeSel]  = { selfgcolor,  selbgcolor,  selbordercolor  },
};
static const unsigned int alphas[][3]      = {
	/*               fg      bg        border     */
	[SchemeNorm] = { OPAQUE, baralpha, borderalpha },
	[SchemeSel]  = { OPAQUE, baralpha, borderalpha },
};

typedef struct {
	const char *name;
	const void *cmd;
} Sp;
const char *spcmd1[] = {TERMINAL, "-n", "spterm", "-g", "120x34", NULL };
const char *spcmd2[] = {TERMINAL, "-n", "spcalc", "-f", "monospace:size=16", "-g", "50x20", "-e", "bc", "-lq", NULL };
static Sp scratchpads[] = {
	/* name          cmd  */
	{"spterm",      spcmd1},
	{"spcalc",      spcmd2},
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };
/* static const char *tags[] = { "🐭", "🐮", "🐯", "🐰", "🐉", "🐍", "🐴", "🐑", "🐵" }; */

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	*/
	/* class    instance      title       	 tags mask    isfloating   isterminal  noswallow  monitor */
	{ "Gimp",     NULL,       NULL,       	    1 << 8,       0,           0,         0,        -1 },
	{ TERMCLASS,  NULL,       NULL,       	    0,            0,           1,         0,        -1 },
  { "ToDesk",   NULL,       NULL,             0,            1,           0,         1,        -1 },
	{ NULL,       NULL,       "Event Tester",   0,            0,           0,         1,        -1 },
  //  this is my personal setting when running python
	{ NULL,       NULL,       "cv2",              0,            0,           0,         1,        -1 },
	{ NULL,      "spterm",    NULL,       	    SPTAG(0),     1,           1,         0,        -1 },
	{ NULL,      "spcalc",    NULL,       	    SPTAG(1),     1,           1,         0,        -1 },
};

/* layout(s) */
static float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static int nmaster     = 1;    /* number of clients in master area */
static int resizehints = 0;    /* 1 means respect size hints in tiled resizals */
#define FORCE_VSPLIT 1  /* nrowgrid layout: force two clients to always split vertically */
#include "vanitygaps.c"
static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",	tile },			/* Default: Master on left, slaves on right */
	{ "TTT",	bstack },		/* Master on top, slaves on bottom */

	{ "[@]",	spiral },		/* Fibonacci spiral */
	{ "[\\]",	dwindle },		/* Decreasing in size right and leftward */

	{ "[D]",	deck },			/* Master on left, slaves in monocle-like mode on right */
	{ "[M]",	monocle },		/* All windows on top of eachother */

	{ "|M|",	centeredmaster },		/* Master in middle, slaves on sides */
	{ ">M>",	centeredfloatingmaster },	/* Same but master floats */

	{ "><>",	NULL },			/* no layout function means floating behavior */
	{ NULL,		NULL },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },
#define STACKKEYS(MOD,ACTION) \
	{ MOD,	XK_k,	ACTION##stack,	{.i = INC(+1) } }, \
	{ MOD,	XK_i,	ACTION##stack,	{.i = INC(-1) } }, \
	{ MOD,  XK_v,   ACTION##stack,  {.i = 0 } }, \
	/* { MOD, XK_grave, ACTION##stack, {.i = PREVSEL } }, \ */
	/* { MOD, XK_a,     ACTION##stack, {.i = 1 } }, \ */
	/* { MOD, XK_z,     ACTION##stack, {.i = 2 } }, \ */
	/* { MOD, XK_x,     ACTION##stack, {.i = -1 } }, */

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static const char *termcmd[]  = { TERMINAL, NULL };

/*
 * Xresources preferences to load at startup
 */
ResourcePref resources[] = {
		{ "color0",		STRING,	&normbordercolor },
		{ "color2",		STRING,	&selbordercolor },  // maybe color1
		/* { "color2",		STRING,	&normbgcolor },    // maybe color1 */
		/* { "color4",		STRING,	&normfgcolor }, */
		/* { "color7",		STRING,	&normfgcolor }, */
		{ "color0",		STRING,	&selfgcolor },
		/* { "color4",		STRING,	&selbgcolor }, */
		{ "borderpx",		INTEGER, &borderpx },
		{ "snap",		INTEGER, &snap },
		{ "showbar",		INTEGER, &showbar },
		{ "topbar",		INTEGER, &topbar },
		{ "nmaster",		INTEGER, &nmaster },
		{ "resizehints",	INTEGER, &resizehints },
		{ "mfact",		FLOAT,	&mfact },
		{ "gappih",		INTEGER, &gappih },
		{ "gappiv",		INTEGER, &gappiv },
		{ "gappoh",		INTEGER, &gappoh },
		{ "gappov",		INTEGER, &gappov },
		{ "swallowfloating",	INTEGER, &swallowfloating },
		{ "smartgaps",		INTEGER, &smartgaps },
};

#include <X11/XF86keysym.h>
#include "shiftview.c"

static Key keys[] = {
	/* modifier                     key        function        argument */
	STACKKEYS(MODKEY,                          focus)
	STACKKEYS(MODKEY|ShiftMask,                push)
	/* { MODKEY|ShiftMask,		XK_Escape,	spawn,	SHCMD("") }, */      // using SHCMD("") to unmap a key
	TAGKEYS(			XK_1,		0)
	TAGKEYS(			XK_2,		1)
	TAGKEYS(			XK_3,		2)
	TAGKEYS(			XK_4,		3)
	TAGKEYS(			XK_5,		4)
	TAGKEYS(			XK_6,		5)
	TAGKEYS(			XK_7,		6)
	TAGKEYS(			XK_8,		7)
	TAGKEYS(			XK_9,		8)
	{ MODKEY,			XK_0,		view,		{.ui = ~0 } },
	{ MODKEY|ShiftMask,		XK_0,		tag,		{.ui = ~0 } },

  // emoji
	{ MODKEY,			XK_grave,	spawn,	SHCMD("dmenuunicode") },
  /* Operation */
	{ MODKEY,			XK_q,		killclient,	{0} },
  { MODKEY|ShiftMask,		XK_q,		spawn,		SHCMD("sysact") },
	{ MODKEY,			XK_Return,	spawn,		{.v = termcmd } },
	{ MODKEY,			XK_d,		spawn,          SHCMD("dmenu_run") },

  /* new a floating window */
	{ MODKEY|ShiftMask,		XK_Return,	togglescratch,	{.ui = 0} },
	{ MODKEY,			XK_apostrophe,	togglescratch,	{.ui = 1} },   // 单引号键, calculator

  /* switch the master window */
	{ MODKEY,			XK_space,	zoom,		{0} },

  /* toggle floating */
	{ MODKEY|ShiftMask,		XK_space,	togglefloating,	{0} },
  /* J and L are automatically bound above in STACKEYS */

	{ MODKEY|ShiftMask,			XK_j,		setmfact,	{.f = -0.05} },
	{ MODKEY|ShiftMask,			XK_l,		setmfact,	{.f = +0.05} },

  /* make a window "sticky"(follow you from tag to tag) */
	{ MODKEY,			XK_s,		togglesticky,	{0} },

  /* toggle the statusbar */
	{ MODKEY|ShiftMask,			XK_b,		togglebar,	{0} },

  /* adjust the gap */
	// { MODKEY,			XK_z,		incrgaps,	{.i = +3 } },                // rarely used
	// { MODKEY,			XK_x,		incrgaps,	{.i = -3 } },                // rarely used

	{ MODKEY,			XK_a,		togglegaps,	{0} },                     // rarely used
	// { MODKEY|ShiftMask,		XK_a,		defaultgaps,	{0} },           // rarely used

  /* full screen */
	{ MODKEY,			XK_f,		togglefullscr,	{0} },

  /* Programs */
	{ MODKEY,			XK_b,		spawn,		SHCMD("$BROWSER") },         // rarely used
	// { MODKEY|ShiftMask,		XK_w,		spawn,		SHCMD(TERMINAL " -e sudo nmtui") },    // modify settings of network, rarely used
	{ MODKEY,			XK_h,		spawn,		SHCMD("translate") },
	{ MODKEY,			XK_r,		spawn,		SHCMD(TERMINAL " -e yazi") },
	{ MODKEY|ShiftMask,		XK_r,		spawn,		SHCMD(TERMINAL " -e bpytop") },

  /* Layouts */    // never used, probably should be removed
	{ MODKEY,			XK_t,		setlayout,	{.v = &layouts[0]} }, /* tile */
	{ MODKEY|ShiftMask,		XK_t,		setlayout,	{.v = &layouts[1]} }, /* bstack */
	// { MODKEY,			XK_y,		setlayout,	{.v = &layouts[2]} }, /* spiral */
	// { MODKEY|ShiftMask,		XK_y,		setlayout,	{.v = &layouts[3]} }, /* dwindle */
	// { MODKEY,			XK_u,		setlayout,	{.v = &layouts[4]} }, /* deck */
	// { MODKEY|ShiftMask,		XK_u,		setlayout,	{.v = &layouts[5]} }, /* monocle */
	// { MODKEY,			XK_n,		setlayout,	{.v = &layouts[6]} }, /* centeredmaster */
	// { MODKEY|ShiftMask,		XK_n,		setlayout,	{.v = &layouts[7]} }, /* centeredfloatingmaster */
	// { MODKEY,			XK_o,		incnmaster,     {.i = +1 } },
	// { MODKEY|ShiftMask,		XK_o,		incnmaster,     {.i = -1 } },
	// { MODKEY|ShiftMask,		XK_f,		setlayout,	{.v = &layouts[8]} },

  /* Tag */
  /* switch previous tag */
	{ MODKEY,			XK_Tab,		view,		{0} },       // never used, keep
	{ MODKEY,			XK_backslash,		view,		{0} },

  /* go prev/next tag */
	{ MODKEY,			XK_j,		shiftview,	{ .i = -1 } },
	{ MODKEY,			XK_l,	shiftview,	{ .i = 1 } },

  /* move current tag to prev/next */
	{ MODKEY|ShiftMask,		XK_h,		shifttag,	{ .i = -1 } },
	{ MODKEY|ShiftMask,		XK_semicolon,	shifttag,	{ .i = 1 } },

  /* monitor movement */
	{ MODKEY,			XK_Left,	focusmon,	{.i = -1 } },
	{ MODKEY|ShiftMask,		XK_Left,	tagmon,		{.i = -1 } },
	{ MODKEY,			XK_Right,	focusmon,	{.i = +1 } },
	{ MODKEY|ShiftMask,		XK_Right,	tagmon,		{.i = +1 } },

  /* System */
	// { MODKEY,			XK_F2,		spawn,		SHCMD(TERMINAL " -e ncmpcpp") },    // never used
	// { MODKEY,			XK_F3,		spawn,		SHCMD("displayselect") },    // never used, probably should map it to another key
  // never used, figure the command
	// { MODKEY,			XK_F4,		spawn,		SHCMD(TERMINAL " -e pulsemixer; kill -44 $(pidof dwmblocks)") },
	// { MODKEY,			XK_F6,		spawn,		SHCMD("torwrap") },
	// { MODKEY,			XK_F7,		spawn,		SHCMD("td-toggle") },
	// { MODKEY,			XK_F8,		spawn,		SHCMD("mw -Y") },
	// { MODKEY,			XK_F9,		spawn,		SHCMD("dmenumount") },
	// { MODKEY,			XK_F10,		spawn,		SHCMD("dmenuumount") },
	// { MODKEY,			XK_F11,		spawn,		SHCMD("mpv --no-cache --no-osc --no-input-default-bindings --profile=low-latency --input-conf=/dev/null --title=webcam $(ls /dev/video[0,2,4,6,8] | tail -n 1)") },
	// { MODKEY,			XK_F12,		spawn,		SHCMD("remaps & notify-send \\\"⌨️ Keyboard remapping...\\\" \\\"Re-running keyboard defaults for any newly plugged-in keyboards.\\\"") },

  /* dmenu option */
	{ MODKEY,			XK_c,		spawn,		SHCMD("dm-confedit") },    // only use it to open TODOList
	{ MODKEY,			XK_m,		spawn,		SHCMD("dm-man") },         // rarely used
	{ MODKEY,			XK_g,		spawn,		SHCMD("$BROWSER www.github.com --new-window") },
	{ MODKEY,			XK_Delete,	spawn,		SHCMD("dm-kill") },
	{ MODKEY,			XK_BackSpace,	spawn,		SHCMD("dm-kill") },  // for keyboard with 67-keys 

  /* Screenshot */
	// { 0,				XK_Print,	spawn,		SHCMD("maimfull") },               // probably map it to another key
	{ 0,				XK_Print,	spawn,		SHCMD("flameshot screen") },               // probably map it to another key
	{ ShiftMask,			XK_Print,	spawn,		SHCMD("flameshot full") },         // probably map it to another key
  // for keyboard with 67-keys 
	// { MODKEY,			XK_x,	spawn,	SHCMD("maimp") },
	{ MODKEY,			XK_x,	spawn,	SHCMD("flameshot gui") },
	// { MODKEY|ShiftMask,			XK_x,	spawn,	SHCMD("maimpick") },
	// { MODKEY,			XK_z,	spawn,	SHCMD("maimc") },  # never used

  // never used, figure the command
	// { MODKEY,			XK_Insert,	spawn,		SHCMD("xdotool type $(grep -v '^#' ~/.local/share/larbs/snippets | dmenu -i -l 50 | cut -d' ' -f1)") },


  /* Audio */  // never used
	{ MODKEY,			XK_p,			spawn,		SHCMD("mpc toggle") },
	{ MODKEY|ShiftMask,		XK_p,			spawn,		SHCMD(TERMINAL " -e ncmpcpp") },
	{ MODKEY,			XK_bracketleft,		spawn,		SHCMD("mpc seek -10") },
	{ MODKEY|ShiftMask,		XK_bracketleft,		spawn,		SHCMD("mpc seek -60") },
	{ MODKEY,			XK_bracketright,	spawn,		SHCMD("mpc seek +10") },
	{ MODKEY|ShiftMask,		XK_bracketright,	spawn,		SHCMD("mpc seek +60") },
	{ MODKEY|ShiftMask,		XK_m,		spawn,		SHCMD("pamixer -t; kill -44 $(pidof dwmblocks)") },
	{ MODKEY,			XK_comma,	spawn,		SHCMD("mpc prev") },
	{ MODKEY|ShiftMask,		XK_comma,	spawn,		SHCMD("mpc seek 0%") },
	{ MODKEY,			XK_period,	spawn,		SHCMD("mpc next") },
	{ MODKEY|ShiftMask,		XK_period,	spawn,		SHCMD("mpc repeat") },
	{ MODKEY,			XK_minus,	spawn,		SHCMD("pamixer --allow-boost -d 5; kill -44 $(pidof dwmblocks)") },
	{ MODKEY|ShiftMask,		XK_minus,	spawn,		SHCMD("pamixer --allow-boost -d 15; kill -44 $(pidof dwmblocks)") },
	{ MODKEY,			XK_equal,	spawn,		SHCMD("pamixer --allow-boost -i 5; kill -44 $(pidof dwmblocks)") },
	{ MODKEY|ShiftMask,		XK_equal,	spawn,		SHCMD("pamixer --allow-boost -i 15; kill -44 $(pidof dwmblocks)") },

	{ 0, XF86XK_AudioMute,		spawn,		SHCMD("pamixer -t; kill -44 $(pidof dwmblocks)") },
	{ 0, XF86XK_AudioRaiseVolume,	spawn,		SHCMD("pamixer --allow-boost -i 3; kill -44 $(pidof dwmblocks)") },
	{ 0, XF86XK_AudioLowerVolume,	spawn,		SHCMD("pamixer --allow-boost -d 3; kill -44 $(pidof dwmblocks)") },
	{ 0, XF86XK_AudioPrev,		spawn,		SHCMD("mpc prev") },
	{ 0, XF86XK_AudioNext,		spawn,		SHCMD("mpc next") },
	{ 0, XF86XK_AudioPause,		spawn,		SHCMD("mpc pause") },
	{ 0, XF86XK_AudioPlay,		spawn,		SHCMD("mpc play") },
	{ 0, XF86XK_AudioStop,		spawn,		SHCMD("mpc stop") },
	{ 0, XF86XK_AudioRewind,	spawn,		SHCMD("mpc seek -10") },
	{ 0, XF86XK_AudioForward,	spawn,		SHCMD("mpc seek +10") },
	{ 0, XF86XK_AudioMedia,		spawn,		SHCMD(TERMINAL " -e ncmpcpp") },
	// { 0, XF86XK_AudioMicMute,	spawn,		SHCMD("pactl set-source-mute @DEFAULT_SOURCE@ toggle") },

  /* Currently the following keys are not used, but they might be useful some day, so just keep these with comments */ 
	// { 0, XF86XK_PowerOff,		spawn,		SHCMD("sysact") },    // lack, never used
	// { 0, XF86XK_Calculator,		spawn,		SHCMD(TERMINAL " -e bc -l") },
	// { 0, XF86XK_Sleep,		spawn,		SHCMD("sudo -A zzz") },    // never used
	// { 0, XF86XK_DOS,		spawn,		SHCMD(TERMINAL) },
	// { 0, XF86XK_ScreenSaver,	spawn,		SHCMD("slock & xset dpms force off; mpc pause; pauseallmpv") },   // lack, never used
	// { 0, XF86XK_TaskPane,		spawn,		SHCMD(TERMINAL " -e htop") },
	// { 0, XF86XK_Mail,		spawn,		SHCMD(TERMINAL " -e neomutt ; pkill -RTMIN+12 dwmblocks") },
	// { 0, XF86XK_MyComputer,		spawn,		SHCMD(TERMINAL " -e lf /") },
	/* { 0, XF86XK_Battery,		spawn,		SHCMD("") }, */
	// { 0, XF86XK_Launch1,		spawn,		SHCMD("xset dpms force off") },
	// { 0, XF86XK_TouchpadToggle,	spawn,		SHCMD("(synclient | grep 'TouchpadOff.*1' && synclient TouchpadOff=0) || synclient TouchpadOff=1") },
	// { 0, XF86XK_TouchpadOff,	spawn,		SHCMD("synclient TouchpadOff=1") },
	// { 0, XF86XK_TouchpadOn,		spawn,		SHCMD("synclient TouchpadOff=0") },
	/* { 0, XF86XK_MonBrightnessUp,	spawn,		SHCMD("xbacklight -inc 15") }, */
	/* { 0, XF86XK_MonBrightnessDown,	spawn,		SHCMD("xbacklight -dec 15") }, */
	// { 0, XF86XK_MonBrightnessUp,	spawn,		SHCMD("light -U 10") },
	// { 0, XF86XK_MonBrightnessDown,	spawn,		SHCMD("light -A 10") },

	/* { MODKEY|Mod4Mask,              XK_h,      incrgaps,       {.i = +1 } }, */
	/* { MODKEY|Mod4Mask,              XK_l,      incrgaps,       {.i = -1 } }, */
	/* { MODKEY|Mod4Mask|ShiftMask,    XK_h,      incrogaps,      {.i = +1 } }, */
	/* { MODKEY|Mod4Mask|ShiftMask,    XK_l,      incrogaps,      {.i = -1 } }, */
	/* { MODKEY|Mod4Mask|ControlMask,  XK_h,      incrigaps,      {.i = +1 } }, */
	/* { MODKEY|Mod4Mask|ControlMask,  XK_l,      incrigaps,      {.i = -1 } }, */
	/* { MODKEY|Mod4Mask|ShiftMask,    XK_0,      defaultgaps,    {0} }, */
	/* { MODKEY,                       XK_y,      incrihgaps,     {.i = +1 } }, */
	/* { MODKEY,                       XK_o,      incrihgaps,     {.i = -1 } }, */
	/* { MODKEY|ControlMask,           XK_y,      incrivgaps,     {.i = +1 } }, */
	/* { MODKEY|ControlMask,           XK_o,      incrivgaps,     {.i = -1 } }, */
	/* { MODKEY|Mod4Mask,              XK_y,      incrohgaps,     {.i = +1 } }, */
	/* { MODKEY|Mod4Mask,              XK_o,      incrohgaps,     {.i = -1 } }, */
	/* { MODKEY|ShiftMask,             XK_y,      incrovgaps,     {.i = +1 } }, */
	/* { MODKEY|ShiftMask,             XK_o,      incrovgaps,     {.i = -1 } }, */

};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
#ifndef __OpenBSD__
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button1,        sigdwmblocks,   {.i = 1} },
	{ ClkStatusText,        0,              Button2,        sigdwmblocks,   {.i = 2} },
	{ ClkStatusText,        0,              Button3,        sigdwmblocks,   {.i = 3} },
	{ ClkStatusText,        0,              Button4,        sigdwmblocks,   {.i = 4} },
	{ ClkStatusText,        0,              Button5,        sigdwmblocks,   {.i = 5} },
	{ ClkStatusText,        ShiftMask,      Button1,        sigdwmblocks,   {.i = 6} },
#endif
	{ ClkStatusText,        ShiftMask,      Button3,        spawn,          SHCMD(TERMINAL " -e nvim ~/.local/src/dwmblocks/config.h") },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        defaultgaps,	{0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkClientWin,		MODKEY,		Button4,	incrgaps,	{.i = +1} },
	{ ClkClientWin,		MODKEY,		Button5,	incrgaps,	{.i = -1} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
	{ ClkTagBar,		0,		Button4,	shiftview,	{.i = -1} },
	{ ClkTagBar,		0,		Button5,	shiftview,	{.i = 1} },
	{ ClkRootWin,		0,		Button2,	togglebar,	{0} },
};
