PKGS += audioresource egl glesv1_cm glib-2.0

CXXFLAGS += -DHAVE_AUDIORESOURCE -DUSE_OPENGL_ES

install: $(TARGET)
	install -m755 -D $(TARGET) $(DESTDIR)$(PREFIX)/bin/$(TARGET)
	mkdir -p $(DESTDIR)$(PREFIX)/share/$(TARGET)/
	cp -rpv Content/* $(DESTDIR)$(PREFIX)/share/$(TARGET)/
	install -m644 -D platform/$(PLATFORM)/$(TARGET).desktop $(DESTDIR)$(PREFIX)/share/applications/$(TARGET).desktop
	install -m644 -D platform/$(PLATFORM)/$(TARGET).png $(DESTDIR)$(PREFIX)/share/icons/hicolor/80x80/apps/$(TARGET).png
.PHONY: install
