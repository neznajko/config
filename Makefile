###____################################################
TARGET   := config
EXTN     := cc
DEBUG    := g3
CXX      := g++
OBJDIR   := obj
SRCDIR   := .
INCDIR   := .
DEPDIR   := dep
CXXFLAGS := -O3 -$(DEBUG) -pedantic -std=c++17 -I$(INCDIR) -pthread
OBJS     := $(addprefix $(OBJDIR)/, config.o comsat.o io.o \
              Search.o hash.o bitboard.o)
DEPS     := $(patsubst $(OBJDIR)/%.o, $(DEPDIR)/%.d, \
              $(OBJS))
########################################################
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^
########################################################
-include $(DEPS)
########################################################
$(OBJDIR)/%.o: $(SRCDIR)/%.$(EXTN)
	$(CXX) $(CXXFLAGS) -o $@ -c $<
	@$(CXX) -MM $(CXXFLAGS) $< > $*.d
	@mv -f $*.d $*.d.tmp
	@sed -e 's|.*:|$@:|' < $*.d.tmp > $*.d
	@cp -f $*.d $*.d.tmp
	@sed -e 's/.*://' -e 's/\\$$//' < $*.d.tmp | \
	fmt -1 | sed -e 's/^ *//' -e 's/$$/:/' >> $*.d
	@rm -f $*.d.tmp
	@mv -f $*.d $(DEPDIR)
########################################################
.PHONY: clean
clean:
	@$(RM) -v $(TARGET) $(OBJS) $(DEPS)
########################################################
# log: http://scottmcpeak.com/autodepend/autodepend.html
########################################################
