TelegramSplit and TelegramMerge are utilities that allow to share on Telegram files which size is bigger than the maximum file size allowed.

TelegramSplit can be used before sending a file which size exceed the maximum size allowed by Telegram.
It splits a file of arbitrary size in parts which size is at most the allowed size and produces a metadata file containing information that will be read by TelegramMerge.


    Usage: TelegramSplit INfilename OUTfilename

    Input Parameters:
    ----------
    INfilename: Name of the input file to be split
    OUTfilename: Filename prefix shared by the output file parts (e.g. INfilename_part)


TelegramMerge can be used after receiving the various parts in order to recombine them in the original file.
It merges back the parts in the original file according to the metafile produced during the split by TelegramSplit.
It expects all of the file parts and metafile in the current working directory.


    Usage: TelegramMerge metafile

    Input Parameters:
    ----------
    metafile: Name of the metafile produced during the split by TelegramSplit

    
    

Compilation instructions:

    gcc TelegramSplit.c -o TelegramSplit
    gcc TelegramMerge.c -o TelegramMerge
