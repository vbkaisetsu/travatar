#ifndef CONFIG_HIERO_EXTRACTOR_RUNNER_H__
#define CONFIG_HIERO_EXTRACTOR_RUNNER_H__

#include <string>
#include <vector>
#include <cstdlib>
#include <sstream>
#include <climits>
#include <travatar/config-base.h>

namespace travatar {

class ConfigHieroExtractorRunner : public ConfigBase {

public:

    ConfigHieroExtractorRunner() : ConfigBase() {
        minArgs_ = 3;
        maxArgs_ = 3;

        SetUsage(
            "~~~ hiero-extractor ~~~\n"
            "  by Philip Arthur\n"
            "\n"
            "Extracts the CFG rule from alligned sentences\n"
            "  Usage: hiero-extractor [SRC] [TRG] [ALIGN]\n"
        );

        AddConfigEntry("initial_phrase_len", "10", "The maximum length of initial phrase in a rule");
        AddConfigEntry("rule_max_len", "5", "The maximum number of words in each extracted rule");
    }
	
};

}

#endif
