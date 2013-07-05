/*
 * This file is subject to the terms of the GFX License. If a copy of
 * the license was not distributed with this file, you can obtain one at:
 *
 *              http://chibios-gfx.com/license.html
 */

/**
 * @file    include/gwin/radio.h
 * @brief   GWIN Graphic window subsystem header file.
 *
 * @defgroup RadioButton RadioButton
 * @ingroup GWIN
 *
 * @details		GWIN allows it to easily create radio buttons with different styles.
 *
 * @pre			GFX_USE_GWIN must be set to TRUE in your gfxconf.h
 * @pre			GWIN_NEED_RADIO must be set to TRUE in your gfxconf.h
 * @{
 */

#ifndef _GWIN_RADIO_H
#define _GWIN_RADIO_H

/* This file is included within "gwin/gwidget.h" */

/**
 * @brief	The Event Type for a Radio Event
 */
#define GEVENT_GWIN_RADIO		(GEVENT_GWIN_FIRST+3)

/**
 * @brief	A Button Event
 * @note	There are currently no GEventGWinRadio listening flags - use 0 as the flags to @p gwinAttachListener()
 */
typedef struct GEventGWinRadio {
	GEventType		type;				// The type of this event (GEVENT_GWIN_RADIO)
	GHandle			radio;				// The radio button that has been depressed
	uint16_t		group;				// The group for this radio button
} GEventGWinRadio;

/**
 * @brief	Button colors
 */
typedef struct GRadioColors {
	color_t				color_edge;
	color_t				color_fill;
	color_t				color_txt;
} GRadioColors;

/**
 * @brief	The radio button widget structure
 * @note	Do not use the members directly - treat it as a black-box.
 */
typedef struct GRadioObject_t {
	GWidgetObject		w;
	#if GINPUT_NEED_TOGGLE
		uint16_t		toggle;
	#endif
	uint16_t			group;
	GRadioColors		c_up;
	GRadioColors		c_dn;
	GRadioColors		c_dis;
} GRadioObject;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief   Create a radio widget.
 * @return  NULL if there is no resultant drawing area, otherwise a window handle.
 *
 * @param[in] gb		The GRadioObject structure to initialise. If this is NULL the structure is dynamically allocated.
 * @param[in] pInit		The initialisation parameters
 * @param[in] group		The group of radio buttons this radio button belongs to.
 *
 * @note				Only one radio button in any group is ever pressed at one time. Pressing one radio button will
 * 						release all others in the group.
 * @note				The drawing color and the background color get set to the current defaults. If you haven't called
 * 						@p gwinSetDefaultColor() or @p gwinSetDefaultBgColor() then these are White and Black respectively.
 * @note				The font gets set to the current default font. If you haven't called @p gwinSetDefaultFont() then there
 * 						is no default font and text drawing operations will no nothing.
 * @note				A radio button remembers its normal drawing state. If there is a window manager then it is automatically
 * 						redrawn if the window is moved or its visibility state is changed.
 * @note				A radio button supports mouse and a toggle input.
 * @note				When assigning a toggle, only one toggle is supported. If you try to assign more than one toggle it will
 * 						forget the previous toggle. When assigning a toggle the role parameter must be 0.
 *
 * @api
 */	
GHandle gwinCreateRadio(GRadioObject *gb, const GWidgetInit *pInit, uint16_t group);

/**
 * @brief   Set the colors of a button.
 *
 * @param[in] gh		The window handle (must be a radio widget)
 * @param[in] pUp		The colors for the button when in the up state.
 * @param[in] pDown		The colors for the button when in the down state.
 * @param[in] pDisabled	The colors for the button when it is disabled.
 *
 * @note				The button is not automatically redrawn. Call gwinButtonDraw() after changing the button style
 * @note				The button style is copied into the internal button structure - there is no need to
 * 						maintain static style structures (they can be temporary structures on the stack).
 * @note				The pUp, pDown and pDisabled parameters can be NULL. If they are then the existing color styles
 * 						are not changed for that button state.
 * @note				Some custom drawn buttons will ignore he specified colors
 *
 * @api
 */
void gwinSetRadioColors(GHandle gh, const GRadioColors *pUp, const GRadioColors *pDown, const GRadioColors *pDisabled);

/**
 * @brief	Press this radio button (and by definition unset any others in the group)
 *
 * @param[in] gh	The window handle (must be a radio widget)
 *
 * @api
 */
void gwinPressRadio(GHandle gh);

/**
 * @brief	Is the radio button currently pressed
 * @return	TRUE if the button is pressed
 *
 * @param[in] gh	The window handle (must be a radio widget)
 *
 * @api
 */
bool_t gwinIsRadioPressed(GHandle gh);

/**
 * @brief	Find the currently pressed radio button in the specified group
 * @return	The handle of the pressed radio button or NULL if none are pressed
 *
 * @param[in] gh	The window handle (must be a radio widget)
 *
 * @api
 */
GHandle gwinActiveRadio(uint16_t group);

/**
 * @brief	Some custom radio button drawing routines
 * @details	These function may be passed to @p gwinSetCustomDraw() to get different radio button drawing styles
 *
 * @param[in] gw		The widget object (in this case a radio button)
 * @param[in] param		A parameter passed in from the user
 *
 * @note				In your custom radio drawing function you may optionally call these
 * 						standard functions and then draw your extra details on top.
 * @note				The standard functions below ignore the param parameter.
 * @note				These custom drawing routines don't have to worry about setting clipping as the framework
 * 						sets clipping to the object window prior to calling these routines.
 *
 * @api
 * @{
 */
void gwinRadioDraw_Radio(GWidgetObject *gw, void *param);					// @< A standard radio button
void gwinRadioDraw_Button(GWidgetObject *gw, void *param);					// @< Draw as a button
void gwinRadioDraw_Tab(GWidgetObject *gw, void *param);						// @< Draw as a tab
/** @} */

#ifdef __cplusplus
}
#endif

#endif /* _GWIN_RADIO_H */
/** @} */

