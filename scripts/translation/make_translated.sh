cat ./CppCoreGuidelines.md | nl | python -c '
from gettext import GNUTranslations;
import sys;
import re;
with open("./scripts/translation/'$1'.mo", "rb") as f:
    translation = GNUTranslations(f)
nlrx = re.compile(r"^\s*([0-9]+)\s(.*)$")
for line in sys.stdin:
    line = nlrx.sub("\\1    \\2", line[:-1])
    print(translation.gettext(line))
' | sed -E 's/([0-9]+)    //g'
