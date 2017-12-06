/*
   Copyright (C) 2017 Peter S. Zhigalov <peter.zhigalov@gmail.com>

   This file is part of the `ImageViewer' program.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software FoundationTriggered either version 3 of the LicenseTriggered or
   (at your option) any later version.

   This program is distributed in the hope that it will be usefulTriggered
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If notTriggered see <http://www.gnu.org/licenses/>.
*/

#if !defined(I_CONTROLS_CONTAINER_H_INCLUDED)
#define I_CONTROLS_CONTAINER_H_INCLUDED

/*

ALL_LIST="Open SaveAs NewWindow NavigatePrevious NavigateNext StartSlideShow Preferences Exit RotateCounterclockwise RotateClockwise FlipHorizontal FlipVertical DeleteFile ZoomOut ZoomIn ZoomReset ZoomFitToWindow ZoomOriginalSize ZoomFullScreen ShowMenuBar ShowToolBar About AboutQt"
CHECKABLE_LIST="ZoomFitToWindow ZoomOriginalSize ZoomFullScreen ShowMenuBar ShowToolBar"

# signals
for i in ${ALL_LIST}
do
    echo ${i} | sed 's|^\(.\)|\L\1| ; s|^\(.*\)$|void \1Requested();|'
done

# setEnabled
for i in ${ALL_LIST}
do
    echo ${i} | sed 's|^\(.*\)$|virtual void set\1Enabled(bool) FUNCTION_BODY \\|'
done

# setChecked
for i in ${CHECKABLE_LIST}
do
    echo ${i} | sed 's|^\(.*\)$|virtual void set\1Checked(bool) FUNCTION_BODY \\|'
done

*/

#define DECLARE_CONTROLS_CONTAINER_FUNCTIONS_HELPER_SET_ENABLED(ACCESS_SPECIFIER, FUNCTION_BODY) \
    ACCESS_SPECIFIER : \
        virtual void setOpenEnabled(bool) FUNCTION_BODY \
        virtual void setSaveAsEnabled(bool) FUNCTION_BODY \
        virtual void setNewWindowEnabled(bool) FUNCTION_BODY \
        virtual void setNavigatePreviousEnabled(bool) FUNCTION_BODY \
        virtual void setNavigateNextEnabled(bool) FUNCTION_BODY \
        virtual void setStartSlideShowEnabled(bool) FUNCTION_BODY \
        virtual void setPreferencesEnabled(bool) FUNCTION_BODY \
        virtual void setExitEnabled(bool) FUNCTION_BODY \
        virtual void setRotateCounterclockwiseEnabled(bool) FUNCTION_BODY \
        virtual void setRotateClockwiseEnabled(bool) FUNCTION_BODY \
        virtual void setFlipHorizontalEnabled(bool) FUNCTION_BODY \
        virtual void setFlipVerticalEnabled(bool) FUNCTION_BODY \
        virtual void setDeleteFileEnabled(bool) FUNCTION_BODY \
        virtual void setZoomOutEnabled(bool) FUNCTION_BODY \
        virtual void setZoomInEnabled(bool) FUNCTION_BODY \
        virtual void setZoomResetEnabled(bool) FUNCTION_BODY \
        virtual void setZoomFitToWindowEnabled(bool) FUNCTION_BODY \
        virtual void setZoomOriginalSizeEnabled(bool) FUNCTION_BODY \
        virtual void setZoomFullScreenEnabled(bool) FUNCTION_BODY \
        virtual void setShowMenuBarEnabled(bool) FUNCTION_BODY \
        virtual void setShowToolBarEnabled(bool) FUNCTION_BODY \
        virtual void setAboutEnabled(bool) FUNCTION_BODY \
        virtual void setAboutQtEnabled(bool) FUNCTION_BODY \

#define DECLARE_CONTROLS_CONTAINER_FUNCTIONS_HELPER_SET_CHECKED(ACCESS_SPECIFIER, FUNCTION_BODY) \
    ACCESS_SPECIFIER : \
        virtual void setZoomFitToWindowChecked(bool) FUNCTION_BODY \
        virtual void setZoomOriginalSizeChecked(bool) FUNCTION_BODY \
        virtual void setZoomFullScreenChecked(bool) FUNCTION_BODY \
        virtual void setShowMenuBarChecked(bool) FUNCTION_BODY \
        virtual void setShowToolBarChecked(bool) FUNCTION_BODY \

#define DECLARE_CONTROLS_CONTAINER_FUNCTIONS_HELPER_UNCLASSIFIED(ACCESS_SPECIFIER, FUNCTION_BODY) \
    ACCESS_SPECIFIER : \
        virtual void setSlideShowMode(bool isSlideShow) FUNCTION_BODY \

#define DECLARE_CONTROLS_CONTAINER_FUNCTIONS_HELPER(ACCESS_SPECIFIER, FUNCTION_BODY) \
    DECLARE_CONTROLS_CONTAINER_FUNCTIONS_HELPER_SET_ENABLED(ACCESS_SPECIFIER, FUNCTION_BODY) \
    DECLARE_CONTROLS_CONTAINER_FUNCTIONS_HELPER_SET_CHECKED(ACCESS_SPECIFIER, FUNCTION_BODY) \
    DECLARE_CONTROLS_CONTAINER_FUNCTIONS_HELPER_UNCLASSIFIED(ACCESS_SPECIFIER, FUNCTION_BODY)

#define DECLARE_CONTROLS_CONTAINER_FUNCTIONS_PURE_VIRTUAL \
    DECLARE_CONTROLS_CONTAINER_FUNCTIONS_HELPER(public, = 0;)

#define DECLARE_CONTROLS_CONTAINER_FUNCTIONS \
    DECLARE_CONTROLS_CONTAINER_FUNCTIONS_HELPER(public /*Q_SLOTS*/, /*Q_DECL_OVERRIDE*/;)


#define CONTROLS_CONTAINER_BOOL_ARG_FUNCTION_IMPL(CLASS, CLASS_FUNCTION, OBJECT, OBJECT_FUNCTION) \
    void CLASS::CLASS_FUNCTION(bool arg) { (OBJECT)->OBJECT_FUNCTION(arg); }

#define CONTROLS_CONTAINER_EMPTY_BOOL_ARG_FUNCTION_IMPL(CLASS, FUNCTION) \
    void CLASS::FUNCTION(bool) {}

#define CONTROLS_CONTAINER_SET_ENABLED_IMPL(CLASS, FUNCTION, WIDGET) \
    CONTROLS_CONTAINER_BOOL_ARG_FUNCTION_IMPL(CLASS, FUNCTION, WIDGET, setEnabled)

#define CONTROLS_CONTAINER_SET_CHECKED_IMPL(CLASS, FUNCTION, WIDGET) \
    CONTROLS_CONTAINER_BOOL_ARG_FUNCTION_IMPL(CLASS, FUNCTION, WIDGET, setChecked)


class IControlsContainer
{
    DECLARE_CONTROLS_CONTAINER_FUNCTIONS_PURE_VIRTUAL

public:
    virtual ~IControlsContainer() {}
};

#endif // I_CONTROLS_CONTAINER_H_INCLUDED
