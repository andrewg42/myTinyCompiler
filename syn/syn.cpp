#include "para.h"
#include "input.h"
#include "parse.h"

int main() {
    std::unique_ptr<Input> input = std::make_unique<Input>();
    input->loadGrammar("grammar2.txt");
    input->loadTokenTable();
    input->buildTokenMap();
    std::unique_ptr<Parse> parse = std::make_unique<Parse>();
    parse->CTRL(std::move(input));
    return 0;
}
