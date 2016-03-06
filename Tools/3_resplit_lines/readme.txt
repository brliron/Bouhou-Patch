Lit les fichiers dialogs*_all_chars.txt, crée une liste chainée de caractères pour chaque paragraphe,
et passe cette liste chainée à CharBuff::dump.
Ceci permet de de reexécuter l'algorithme de séparation de lignes en ayant plus d'informations sur les caractères,
ou en testant avec un autre algorithme.
Le résultat est stocké dans dialogs*_all_chars_v2.txt (les noms commencent à être bien longs...)

Aussi, CharBuff est initialisé sans Log::PRINT_ALL_CHARS ni Log::PRINT_TRANSLATION, donc la sortie contient
beaucoup moins de textes.
