# -------------------------------------------------
# Project created by QtCreator 2010-08-22T14:12:19
# -------------------------------------------------
QT += opengl xml
TARGET = final
TEMPLATE = app

QMAKE_CXXFLAGS += -std=c++14
CONFIG += c++14

win32 {
    DEFINES += GLEW_STATIC
    LIBS += -lopengl32 -lglu32
}

macx {
    QMAKE_CFLAGS_X86_64 += -mmacosx-version-min=10.7
    QMAKE_CXXFLAGS_X86_64 = $$QMAKE_CFLAGS_X86_64
    CONFIG += c++11
}

linux {
    # fixes an issue with linux and vr libraries
    QMAKE_CXXFLAGS += -D_GLIBCXX_USE_CXX11_ABI=0
}

SOURCES += \
    gl/shaders/Shader.cpp \
    gl/GLDebug.cpp \
    gl/datatype/VBOAttribMarker.cpp \
    gl/datatype/VBO.cpp \
    gl/datatype/IBO.cpp \
    gl/datatype/VAO.cpp \
    gl/datatype/FBO.cpp \
    gl/textures/Texture.cpp \
    gl/textures/Texture2D.cpp \
    gl/textures/TextureParameters.cpp \
    gl/textures/TextureParametersBuilder.cpp \
    gl/textures/RenderBuffer.cpp \
    gl/textures/DepthBuffer.cpp \
    gl/shaders/CS123Shader.cpp \
    gl/shaders/GBufferShader.cpp \
    gl/util/FullScreenQuad.cpp \
    lib/CS123XmlSceneParser.cpp \
    lib/ResourceLoader.cpp \
    scenegraph/Scene.cpp \
    scenegraph/OpenGLScene.cpp \
    scenegraph/SceneviewScene.cpp \
    ui/view.cpp \
    ui/viewformat.cpp \
    ui/mainwindow.cpp \
    glew-1.10.0/src/glew.c \
    main.cpp \
    camera/QuaternionCamera.cpp \
    shapes/cone.cpp \
    shapes/Cube.cpp \
    shapes/cylinder.cpp \
    shapes/OpenGLShape.cpp \
    shapes/shapeutil.cpp \
    shapes/sphere.cpp \
    shapes/squareringdrawer.cpp \
    shapes/trianglestripdrawer.cpp \
    shapes/column.cpp \
    scenegraph/waluigiscene.cpp \
    shapes/square.cpp

HEADERS += \
    gl/shaders/Shader.h \
    gl/GLDebug.h \
    gl/shaders/ShaderAttribLocations.h \
    gl/datatype/VBOAttribMarker.h \
    gl/datatype/VBO.h \
    gl/datatype/IBO.h \
    gl/datatype/VAO.h \
    gl/datatype/FBO.h \
    gl/textures/Texture.h \
    gl/textures/Texture2D.h \
    gl/textures/TextureParameters.h \
    gl/textures/TextureParametersBuilder.h \
    gl/textures/RenderBuffer.h \
    gl/textures/DepthBuffer.h \
    gl/shaders/CS123Shader.h \
    gl/shaders/GBufferShader.h \
    gl/util/FullScreenQuad.h \
    lib/CS123XmlSceneParser.h \
    lib/CS123SceneData.h \
    lib/CS123ISceneParser.h \
    lib/ResourceLoader.h \
    scenegraph/Scene.h \
    scenegraph/OpenGLScene.h \
    scenegraph/SceneviewScene.h \
    ui/view.h \
    ui/viewformat.h \
    ui/mainwindow.h \
    glew-1.10.0/include/GL/glew.h \
    camera/QuaternionCamera.h \
    shapes/cone.h \
    shapes/Cube.h \
    shapes/cylinder.h \
    shapes/OpenGLShape.h \
    shapes/shapeutil.h \
    shapes/sphere.h \
    shapes/squareringdrawer.h \
    shapes/trianglestripdrawer.h \
    shapes/column.h \
    scenegraph/waluigiscene.h \
    shapes/square.h

DEFINES += _USE_MATH_DEFINES
DEFINES += TIXML_USE_STL
DEFINES += GLM_SWIZZLE GLM_FORCE_RADIANS
OTHER_FILES += \
    shaders/* \
    shaders/texture.frag \
    shaders/texture.vert

FORMS += ui/mainwindow.ui
INCLUDEPATH += libraries\openvr\lib\win64 glm lib libraries libraries/openvr/headers shapes ui glew-1.10.0/include
DEPENDPATH += glm lib libraries libraries/openvr/headers shapes ui glew-1.10.0/include

DEFINES += _USE_MATH_DEFINES
DEFINES += TIXML_USE_STL
DEFINES += GLM_SWIZZLE GLM_FORCE_RADIANS
OTHER_FILES += \
    shaders/*

RESOURCES += \
    resources.qrc

defineTest(copyToDestdir) {
    files = $$1
    for(FILE, files) {
        CONFIG(debug, debug|release) {
            DDIR = $${OUT_PWD}/debug
        } else {
            DDIR = $${OUT_PWD}/release
        }

        # Replace slashes in paths with backslashes for Windows
        win32:FILE ~= s,/,\\,g
        win32:DDIR ~= s,/,\\,g
        QMAKE_POST_LINK += $$QMAKE_COPY $$quote($$FILE) $$quote($$DDIR) $$escape_expand(\\n\\t)
    }
    export(QMAKE_POST_LINK)
}

win32 {
    contains(QT_ARCH, i386) {
        message("32 bit build")
            LIBS += -L$$PWD/libraries/openvr/lib/win32/ -lopenvr_api
            copyToDestdir($$PWD/libraries/openvr/bin/win32/openvr_api.dll)
    } else {
        message("64 bit build")
            LIBS += -L$$PWD/libraries/openvr/lib/win64/ -lopenvr_api
            copyToDestdir($$PWD/libraries/openvr/bin/win64/openvr_api.dll)
    }
}

macx {
    LIBS += -L$$PWD/libraries/openvr/bin/osx32 -lopenvr_api
}

linux {

    LIBS += -L$$PWD/libraries/steam-runtime/amd64/lib/x86_64-linux-gnu
    LIBS += -L$$PWD/libraries/steam-runtime/amd64/lib
    LIBS += -L$$PWD/libraries/steam-runtime/amd64/usr/lib/x86_64-linux-gnu
    LIBS += -L$$PWD/libraries/steam-runtime/amd64/usr/lib
    LIBS += -L$$PWD/libraries/steam-runtime/i386/lib/i386-linux-gnu
    LIBS += -L$$PWD/libraries/steam-runtime/i386/lib
    LIBS += -L$$PWD/libraries/steam-runtime/i386/usr/lib/i386-linux-gnu
    LIBS += -L$$PWD/libraries/steam-runtime/i386/usr/lib
    LIBS += -L$$PWD/libraries/openvr/bin/linux64 -lopenvr_api
}
