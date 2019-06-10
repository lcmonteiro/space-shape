#!/usr/bin/env python3
# -*- coding: utf-8 -*-
#-----------------------------------------------------------------------------------------------------------------------
# dependencies
#-----------------------------------------------------------------------------------------------------------------------
from argparse    import ArgumentParser
#from googletrans import Translator
from translate   import Translator
from random      import randint
from time        import sleep
from json        import load
from json        import dump
from sys         import exit
#-----------------------------------------------------------------------------------------------------------------------
# tools
#-----------------------------------------------------------------------------------------------------------------------
def split(l, s):
    for i in range(0, len(l), s):
        yield l[i:i+s]
#-----------------------------------------------------------------------------------------------------------------------
# main
#-----------------------------------------------------------------------------------------------------------------------
def main(param):
    translator = Translator(from_lang=param.source, to_lang=param.destination)
    # process
    with open(param.input) as ifile:
        # load
        data = load(ifile)
        # translate
        translated = []
        for d in data:
            t = translator.translate(d)
            print(t)
            translated.append(t)
        # save
        with open(param.output, 'w') as ofile:  
            dump(translated, ofile)
    return 0
#-----------------------------------------------------------------------------------------------------------------------
# entry point
#-----------------------------------------------------------------------------------------------------------------------
if __name__ == '__main__':
    
    # -------------------- options -----------------------------------------------
    parser = ArgumentParser(description="Translate")
    parser.add_argument('-s','--source',      help= 'source language',      default= 'auto')
    parser.add_argument('-d','--destination', help= 'destination language', default= 'en'  )
    parser.add_argument('-i','--input',       help= 'input file',                          )
    parser.add_argument('-o','--output',      help= 'output file',                         )

    # --------------- main ---------------------------------------------------------
    try:
        res = main(parser.parse_args())
    except Exception as inst: 
        print(str(inst))
        exit(-1)
    #
    exit(res)
#-----------------------------------------------------------------------------------------------------------------------
# end
#-----------------------------------------------------------------------------------------------------------------------