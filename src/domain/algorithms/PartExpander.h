#include "../entities/ExpandedPart.h"
#include "../entities/Part.h"
#include <vector>

class PartExpander
{
public:
    static std::vector<ExpandedPart>
    expand(const std::vector<Part>& parts);
};