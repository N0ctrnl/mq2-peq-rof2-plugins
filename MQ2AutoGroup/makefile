!include "../global.mak"

ALL : "$(OUTDIR)\MQ2AutoGroup.dll"

CLEAN :
	-@erase "$(INTDIR)\MQ2AutoGroup.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\MQ2AutoGroup.dll"
	-@erase "$(OUTDIR)\MQ2AutoGroup.exp"
	-@erase "$(OUTDIR)\MQ2AutoGroup.lib"
	-@erase "$(OUTDIR)\MQ2AutoGroup.pdb"


LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib $(DETLIB) ..\Release\MQ2Main.lib /nologo /dll /incremental:no /pdb:"$(OUTDIR)\MQ2AutoGroup.pdb" /debug /machine:I386 /out:"$(OUTDIR)\MQ2AutoGroup.dll" /implib:"$(OUTDIR)\MQ2AutoGroup.lib" /OPT:NOICF /OPT:NOREF 
LINK32_OBJS= \
	"$(INTDIR)\MQ2AutoGroup.obj" \
	"$(OUTDIR)\MQ2Main.lib"

"$(OUTDIR)\MQ2AutoGroup.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) $(LINK32_FLAGS) $(LINK32_OBJS)


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("MQ2AutoGroup.dep")
!INCLUDE "MQ2AutoGroup.dep"
!ELSE 
!MESSAGE Warning: cannot find "MQ2AutoGroup.dep"
!ENDIF 
!ENDIF 


SOURCE=.\MQ2AutoGroup.cpp

"$(INTDIR)\MQ2AutoGroup.obj" : $(SOURCE) "$(INTDIR)"

