PKGS += glib-2.0

ifeq ($(shell uname),Darwin)
LIBS += -framework OpenGL
else
PKGS += gl
endif

install: $(TARGET)
	install -m755 -D $(TARGET) $(DESTDIR)$(PREFIX)/bin/$(TARGET)
	mkdir -p $(DESTDIR)$(PREFIX)/share/$(TARGET)/
	cp -rpv Content/* $(DESTDIR)$(PREFIX)/share/$(TARGET)/
	mkdir -p $(DESTDIR)$(PREFIX)/share/applications/
	sed -e 's#brainparty#$(TARGET)#' platform/$(PLATFORM)/brainparty.desktop >$(DESTDIR)$(PREFIX)/share/applications/$(TARGET).desktop
	install -m644 -D platform/$(PLATFORM)/brainparty.png $(DESTDIR)$(PREFIX)/share/icons/hicolor/64x64/apps/$(TARGET).png
.PHONY: install
