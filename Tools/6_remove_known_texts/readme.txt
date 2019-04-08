********** English *********
Read the files dialogs*_all_chars_v2 - jp sentences.txt, and remove the known texts.
For example, in a playthrough, we would remove the intro and the menus.
The known texts are loaded from a list of hardcoded files - edit the source file to
add a file to the list.
It also removes useless comments. For example, if we have ## Loading stage 2 then
## Loading stage 3, it will keep only ## Load stage 3.
The output will be in dialogs* - jp sentences.txt (because it's time to clean this
file name a bit).

********** French **********
Lit les fichiers dialogs*_all_chars_v2 - jp sentences.txt, et supprime les phrases déjà connues.
Par exemple, dans un playthrough, on supprimera l'intro et les menus.
Les phrases déjà connues sont chargées depuis dialogs - intro - jp sentences.txt et
dialogs - items - jp sentences.txt.
Supprime également les commentaires inutiles : si on a ## Loading stage 2 puis ## Loading stage 3,
on ne gardera que ## Loading stage 3.
Le résultat est stocké dans dialogs* - jp sentences.txt (parce qu'il est temps de nettoyer le nom)
