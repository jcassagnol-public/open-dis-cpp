#!/bin/bash
# see https://nlohmann.github.io/json/features/arbitrary_types/ for arbitrary type conversions to json
# the premise of this shell script is we have the byte level marshal and unmarshal functions, and we're
# using those functions as to autogen the associated JSON for them.
# if we create the to_json and from_json functions, they can be called recursively and the parser is easier to work with.
for i in ./src/dis6/*.cpp; do
    echo "$i";
    # create the to_json function --------------------------------------------------------------------------------

    # 1. match the marshal line from the first { to the closing } on the final line. Print that whole match out.
    # 2. convert 'dataString << (unsigned) _memberVariable;' to 'json_stream["memberVariable"] = toConvert._memberVariable;'
    # 3. convert 'dataString << (unsigned) _memberVariable.size();' to 'json_stream["memberVariable"] = toConvert._memberVariable;' (note: relies on built-in conversion of std::vector by nlohmann)
    # 4. convert 'dataStream << this->getFieldName()' to 'jsonStream["fieldName"] = toConvert.getFieldName()' (uses GNU extension to set F in FieldName to lowercase, see \l, https://stackoverflow.com/a/24042315/7818349)
    # 5. convert '_memberVariable.marshal()' to 'json_stream["memberVariable"] = toConvert._memberVariable'
    # 6. convert 'ParentClass::marshal()' to '// Convert the base class "ParentClass" to JSON first
    #                                         auto parent_stream = nlohmann::json::object();
    #                                         to_json(parent_stream, static_cast<const ParentClass&>(toConvert));'
    # 7. convert 'void ThisClass::marshall.*' to 'void DIS::to_json(nlohmann::json\& json-stream, const ThisClass &toConvert)'
    # 8. place a placeholder, 'replace_me_with_parent_or_null_later' for later deletion or replacement with 'parent_stream["ThisClass"] = json_stream;
    #                                                                                                        json_stream = std::move(parent_stream);'.
    # 9. convert the for loops for variable datums type objects to only have their bodies, but strip the loops already converted in step 3. The loops we already converted are the ones where we use temp variables and .marshal().
    # 10. convert the remaining << variable datums to toHex calls, as they're all either static char byte arrays or vectors of char.
    to_json_function=$(awk 'BEGIN { p = 0 }; /::marshal/ { print; p = 1; next }; p { print }; /^}/ { p = 0; next };' $i |
        sed -re 's/dataStream << (\(.*\))?_([^\[\.]*);/json_stream["\2"] = toConvert._\2;/' |
        sed -re 's/dataStream << (\(.*\))?_([^\[\.]*)\.size\(\);/json_stream["\2"] = toConvert._\2;/' |
        sed -re 's/dataStream << this->get(.*)\(\);/json_stream["\l\1"] = toConvert.get\1();/' |
        sed -re 's/_(.*)\.marshal\(dataStream\);/json_stream["\1"] = toConvert._\1;/' |
        sed -re 's|(\w+)::marshal\(dataStream\).*|// Convert the base class "\1" to JSON first\n    auto parent_stream = nlohmann::json::object();\n    to_json(parent_stream, static_cast<const \1\&>(toConvert));\n|' |
        sed -re 's/void (.*)::marshal.* const/void DIS::to_json(nlohmann::json\& json_stream, const \1\& toConvert)/' |
        sed -re 's/^}/    replace_me_with_parent_or_null_later;\n}/' |
        awk 'BEGIN { f = 0; p = 1 }; /for\(/ { p = 0; f = 1; next }; /}/ && f { f = 0; p = 0; next }; p && (!f || !(/\.marshal/ || /= *_/)) { print }; !p { p = 1 };' |
        sed -re 's/\s+dataStream\s*<<\s*_(\w+).*/    json_stream["\1"] = toHex(toConvert._\1);/');

    # declare json_stream if we're not inheriting from the parent class
    # this is a temp object that will replace whatever the output stream was passed into us. We could just use the output parameter directly instead.
    # currently I have this to be explicit but technically the ::object part isn't needed (could be replaced with json_stream if we wanted to)
    if ! printf '%s' "${to_json_function}" | grep -q -e "auto parent_stream"; then
        to_json_function=$(printf '%s' "${to_json_function}" |
            sed -re 's/^\{/{\n    json_stream = nlohmann::json::object();/' |
            sed -re '/.*replace_me_with_parent_or_null_later.*/d');
    # parent classes on the other hand have to set the output as the _parent_ stream, as this current parser is a sub-element of the parent stream.
    # if we set the output parameter to _our_ code, what happens is the parent object will get destructed and we'll have bad references to our object!
    # so we set the output to the parent stream and our stream gets put as a sub-stream in the parent. Phew.
    else
        # this finds the current name of the class we're dealing with, stealing it from the to_json definition
        class_name=$(printf '%s' "${to_json_function}" |
            grep -e 'void DIS::to_json(nlohmann::json' |
            sed -re "s/.*, const (\w+).*/\1/");

        to_json_function=$(printf '%s' "${to_json_function}" |
            sed -re "s/replace_me_with_parent_or_null_later;/parent_stream[\"${class_name}\"] = std::move(json_stream);\n    json_stream = std::move(parent_stream);/");
    fi;

    # this replaces the temp stand-in from the original generated code with the real class name that we looked up a second ago.
    # to_json_function=$(printf '%s' "${to_json_function}" |
    #     sed -re "s/replace_me_with_normal_classname_later/${class_name}/");
    
    # escape the generated code, replacing references to newlines with \n, replace " with \"
    escaped_to_json_function=$(printf "%s" "$to_json_function" | sed ':a;N;$!ba;s/\n/\\n/g' | sed -re 's/"/\\"/g');

    # inplace the resulting to_json before the copyright notice at the end of the file.
    awk -i inplace "/\/\/ Copyright/ { print \"${escaped_to_json_function}\n\"; print; next }1" $i;

    # end the to_json function -----------------------------------------------------------------------------------
    # create the from_json function ------------------------------------------------------------------------------

    # 1. match the marshal line from the first { to the closing } on the final line. Print that whole match out.
    # 2. convert 'dataString << (unsigned) _memberVariable;' to 'toConvert._memberVariable = json_stream["memberVariable"];'
    # 3. convert 'dataString << (unsigned) _memberVariable.size();' to 'toConvert._memberVariable = json_stream["memberVariable"];' (note: relies on built-in conversion of std::vector by nlohmann)
    # 4. delete 'dataStream << this->getFieldName()'
    # 5. convert '_memberVariable.marshal()' to 'toConvert._memberVariable = json_stream["memberVariable"]'
    # 6. convert 'ParentClass::marshal()' to '// Convert the base class "ParentClass" to JSON first
    #                                         from_json(json_stream, static_cast<ParentClass>(toConvert));'
    # 7. convert 'void ThisClass::marshall.*' to 'void DIS::from_json(const nlohmann::json\& json-stream, ThisClass &toConvert)'
    # 8. convert the for loops for variable datums type objects to only have their bodies, but strip the loops already converted in step 3. The loops we already converted are the ones where we use temp variables and .marshal().
    # 9. convert the remaining << variable datums to fromHex calls, as they're all either static char byte arrays or vectors of char.
    from_json_function=$(awk 'BEGIN { p = 0 }; /::marshal/ { print; p = 1; next }; p { print }; /^}/ { p = 0; next };' $i |
        sed -re 's/dataStream << (\(.*\))?_([^\[\.]*);/json_stream.at("\2").get_to(toConvert._\2);/' |
        sed -re 's/dataStream << (\(.*\))?_([^\[\.]*)\.size\(\);/json_stream.at("\2").get_to(toConvert._\2);/' |
        sed -re '/dataStream << this->get(.*)\(\);/d' |
        sed -re 's/_(.*)\.marshal\(dataStream\);/json_stream.at("\1").get_to(toConvert._\1);/' |
        sed -re 's|(\w+)::marshal\(dataStream\).*|// Convert the base class "\1" from JSON first\n    from_json(json_stream, static_cast<\1\&>(toConvert));\n|' |
        sed -re 's/void (.*)::marshal.* const/void DIS::from_json(const nlohmann::json\& json_stream, \1\& toConvert)/' |
        awk 'BEGIN { f = 0; p = 1 }; /for\(/ { p = 0; f = 1; next }; /}/ && f { f = 0; p = 0; next }; p && (!f || !(/\.marshal/ || /= *_/)) { print }; !p { p = 1 };' |
        sed -re 's/\s+dataStream\s*<<\s*_(\w+).*/    fromHex(json_stream["\1"], toConvert._\1);/');
    
    # escape the generated code, replacing references to newlines with \n, replace " with \"
    escaped_from_json_function=$(printf "%s" "$from_json_function" | sed ':a;N;$!ba;s/\n/\\n/g' | sed -re 's/"/\\"/g');

    # inplace the resulting from_json before the copyright notice at the end of the file.
    awk -i inplace "/\/\/ Copyright/ { print \"${escaped_from_json_function}\n\"; print; next }1" $i;
    # end the from_json function ---------------------------------------------------------------------------------
done

for i in ./src/dis6/*.h; do
    echo "$i";
    filename=$(basename "$i")

    # grabs the class name from the destructor inside the header file
    class_name=$(grep -E -e "~ *[A-Za-z0-9]+ *\(\)" $i |
        sed -re "s/.*~ *([A-Za-z0-9]+) *\(.*/\1/");

    # don't add nlohmann to the Pdu headers, as they'll inherit from their base classes.
    if ! [[ $filename =~ Pdu ]]; then
        # inplace the include before the namespace
        awk -i inplace '/namespace DIS/ { print "#include <nlohmann/json.hpp>\n#include <utils/HexProcessor.h>\n\n"; print; next }1' $i;
    elif [[ $filename == Pdu.h ]] || [[ $filename == PduSuperclass.h ]]; then
        # inplace the include before the namespace
        awk -i inplace '/namespace DIS/ { print "#include <nlohmann/json.hpp>\n#include <utils/HexProcessor.h>\n\n"; print; next }1' $i;
    fi

    # insert the function defs! Yay!
    awk -i inplace "/virtual void unmarshal/ { print; print \"    friend void to_json(nlohmann::json& json_stream, const $class_name& toConvert);\n    friend void from_json(const nlohmann::json& json_stream, $class_name& toConvert);\"; next}1" $i;

    # insert the pre-declarations for the functions so they appear in the correct namespace! Yay!
    awk -i inplace "BEGIN { n = 0 }; { print }; /class/ && n == 0 { n = 1 }; /^};/ && n == 1 { n = 999; print \"\n// Declare the to_json and from_json functions in the DIS namespace as\n// these are not class members and the friend declaration does not do this for us\nvoid to_json(nlohmann::json& json_stream, const $class_name& toConvert);\nvoid from_json(const nlohmann::json& json_stream, $class_name& toConvert);\"; }" $i;
done