
all: lzma lz4 lzo zlib zstd
	@echo $(CONFIGURATION) $(PLATFORM) 

clean: clean_lzma clean_lz4 clean_lzo clean_zlib clean_zstd
	@echo CLEAN $(CONFIGURATION) $(PLATFORM) $(TARGET)


rebuild:
	@nmake /nologo clean
	@nmake /nologo 


lzma:
	@build_lzma.cmd $(CONFIGURATION) $(PLATFORM)

lz4:
	@build_lz4.cmd $(CONFIGURATION) $(PLATFORM)

lzo:
	@build_lzo.cmd $(CONFIGURATION) $(PLATFORM)

zlib:
	@build_zlib.cmd $(CONFIGURATION) $(PLATFORM)

zstd:
	@build_zstd.cmd $(CONFIGURATION) $(PLATFORM)


clean_lzma:
	@clean_lzma.cmd $(CONFIGURATION) $(PLATFORM)

clean_lz4:
	@clean_lz4.cmd $(CONFIGURATION) $(PLATFORM)

clean_lzo:
	@clean_lzo.cmd $(CONFIGURATION) $(PLATFORM)

clean_zlib:
	@clean_zlib.cmd $(CONFIGURATION) $(PLATFORM)

clean_zstd:
	@clean_zstd.cmd $(CONFIGURATION) $(PLATFORM)
