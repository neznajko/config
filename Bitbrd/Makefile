###____################################################
TARGET   := config
EXTN     := cc
DEBUG    := g3
CXX      := g++
OBJDIR   := obj
SRCDIR   := .
INCDIR   := .
DEPDIR   := dep
CXXFLAGS := -O3 -$(DEBUG) -pedantic -std=c++20 \
            -I$(INCDIR) -pthread
OBJS     := $(addprefix $(OBJDIR)/, config.o \
bitboard.o io.o comsat.o picker.o search.o)
DEPS     := $(patsubst $(OBJDIR)/%.o, $(DEPDIR)/%.d, $(OBJS))
#######################################################
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^
#######################################################
-include $(DEPS)
#######################################################
$(OBJDIR) $(DEPDIR):
	mkdir -p $@
#######################################################
$(OBJDIR)/%.o: $(SRCDIR)/%.$(EXTN) | $(OBJDIR) $(DEPDIR)
	$(CXX) $(CXXFLAGS) -MT $@ -MMD -MP -MF $(DEPDIR)/$*.d -c $< -o $@
#######################################################
.PHONY: clean
clean:
	@$(RM) -v $(TARGET) $(TARGET).exe $(OBJS) $(DEPS)
#######################################################
# log: wooo
#######################################################
