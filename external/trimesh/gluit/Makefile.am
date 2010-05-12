# Find config.h.
AM_CPPFLAGS = -I$(top_builddir)
AM_CPPFLAGS += -I$(top_srcdir)/include -I$(top_srcdir)/include/GL
AM_LDFLAGS = -lGL -lGLU -lX11

lib_LTLIBRARIES = libgluit.la

libgluit_la_SOURCES =				\
  glui_algebra3.h				\
  glui_arcball.h				\
  glui_quaternion.h				\
  glui_stdinc.h					\
						\
  glui.cc					\
  glui_add_controls.cc				\
  glui_algebra3.cc				\
  glui_arcball.cc				\
  glui_bitmap_img_data.cc			\
  glui_bitmaps.cc				\
  glui_button.cc				\
  glui_checkbox.cc				\
  glui_column.cc				\
  glui_control.cc				\
  glui_edittext.cc				\
  glui_listbox.cc				\
  glui_mouse_iaction.cc				\
  glui_node.cc					\
  glui_panel.cc					\
  glui_quaternion.cc				\
  glui_radio.cc					\
  glui_rollout.cc				\
  glui_rotation.cc				\
  glui_separator.cc				\
  glui_slider.cc				\
  glui_spinner.cc				\
  glui_statictext.cc				\
  glui_translation.cc

# These files are actually header, despite their `.c' extension!
noinst_HEADERS =				\
  glui_img_checkbox_0.c				\
  glui_img_checkbox_0_dis.c			\
  glui_img_checkbox_1.c				\
  glui_img_checkbox_1_dis.c			\
  glui_img_downarrow.c				\
  glui_img_leftarrow.c				\
  glui_img_listbox_down.c			\
  glui_img_listbox_up.c				\
  glui_img_listbox_up_dis.c			\
  glui_img_radiobutton_0.c			\
  glui_img_radiobutton_0_dis.c			\
  glui_img_radiobutton_1.c			\
  glui_img_radiobutton_1_dis.c			\
  glui_img_rightarrow.c				\
  glui_img_spindown_0.c				\
  glui_img_spindown_1.c				\
  glui_img_spindown_dis.c			\
  glui_img_spinup_0.c				\
  glui_img_spinup_1.c				\
  glui_img_spinup_dis.c				\
  glui_img_uparrow.c

# These files are not needed on Darwin (Mac OS X).
if ! DARWIN
  libgluit_la_SOURCES +=			\
    freeglut_internal.h				\
    freeglut_teapot_data.h			\
						\
    freeglut_callbacks.c			\
    freeglut_cursor.c				\
    freeglut_display.c				\
    freeglut_ext.c				\
    freeglut_font.c				\
    freeglut_font_data.c			\
    freeglut_gamemode.c				\
    freeglut_geometry.c				\
    freeglut_glutfont_definitions.c		\
    freeglut_init.c				\
    freeglut_joystick.c				\
    freeglut_main.c				\
    freeglut_menu.c				\
    freeglut_misc.c				\
    freeglut_overlay.c				\
    freeglut_state.c				\
    freeglut_stroke_mono_roman.c		\
    freeglut_stroke_roman.c			\
    freeglut_structure.c			\
    freeglut_teapot.c				\
    freeglut_videoresize.c			\
    freeglut_window.c
endif ! DARWIN
