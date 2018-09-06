./scripts/translation/parse_source_md.sh |
sed -E 's/^\s*([0-9]+)\s(.*)$/# CppCoreGuidelines.md:\1\nmsgid "\1    \2"\nmsgstr""\n/g' |
cat ./scripts/translation/CppCoreGuidelines.pot.header - > ./scripts/translation/CppCoreGuidelines.pot
