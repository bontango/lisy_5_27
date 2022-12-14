##########################################################################
#
#  romalizer translation file from ENGLISH --- to ---> SPANISH by:
#  traducci?n del romalizer de INGL?S --- a ---> ESPA?OL por:
#
#     Vicente Aguilar <vaguilar@linuxfreak.com>
#     http://emuhq.com/mamelang, http://pagina.de/bisente
#
#  The Romalizer, by Christopher Stone (tkmame@ztnet.com)
#  http://tkmame.retrogames.com
#
##########################################################################
"The Romalizer 0.62 (May 27, 2000)\nChristopher Stone (tkmame@ztnet.com)\nhttp://tkmame.retrogames.com\n"
"The Romalizer 0.62 (May 27, 2000)\nChristopher Stone (tkmame@ztnet.com)\nhttp://tkmame.retrogames.com\n"
"  NAME"
"  NOMBRE"
"         romalizer - analyze and fix ROMs used for xmame"
"         romalizer - analiza y corrige las ROMs del xmame"
"  SYNOPSIS"
"  SINOPSIS"
"         romalizer [-f] [-g] [-h] [-m] [-n] [-v] [-G] [-V] [-c level]"
"         romalizer [-f] [-g] [-h] [-m] [-n] [-v] [-G] [-V] [-c nivel]"
"         [-d workdir] [-l logfile] [-r rompath] [-x xmame] [-C cutoff]"
"         [-d dir_trabajo] [-l bit?cora] [-r dir_roms] [-x xmame] [-C m?nimo]"
"         ROM ..."
"         ROM ..."
"  DESCRIPTION"
"  DESCRIPCI?N"
"         romalizer will examine the  ROM(s) specified on the command line"
"         romalizer examinar? la(s)  ROM(s) especificadas en l?nea de comandos"
"         as well as all the  clones for each ROM.  If ROM is a clone then"
"         as? como todos los clones de cada una de ellas. Si la ROM es un clon"
"         the   associated parent   ROM is used   instead, any   resulting"
"         se  utilizar?  en  su lugar  la  ROM  original  asociada,  ignorando"
"         duplicate ROMs  will be ignored, thus using  wildcards such as *"
"         cualquier duplicado resultante de forma que al usar comodines como *"
"         will not result in reexamining the same ROM twice.  The ROM that"
"         no se reexamine  la misma ROM dos veces.  El nombre de la ROM que se"
"         is specified  may be the short name  recognized by xmame, or the"
"         especifica puede  ser el nombre corto reconocide por el xmame,  o el"
"         full path name to the zip file."
"         camino completo hasta el archivo zip."
"         During the examination process, romlizer compares the CRCs found"
"         Durante  el proceso  de  an?lisis,  romalizer compara  los  CRCs que"
"         in xmame with  those in your zip files.  If any extra  files are"
"         necesita el xmame con  los de sus archivos zip.  Si hubiera ficheros"
"         stored  in your zip files,  romalizer detects them  and issues a"
"         de m?s en  los archivos zip,  romalizer los detectar? y generar?  un"
"         report.  romalizer is  also capable  of  building  new zip files"
"         informe.  Romalizer tambi?n  es capaz de crear  nuevos archivos  zip"
"         from your existing zip files if any discrepancies are found."
"         a partir de los que ya tiene si encuentra alguna discrepancia."

"         ROM sets are  typically divided into a parent  ROM and its clone"
"         Los conjuntos  de ROMs se dividen  generalmente en una  ROM original"
"         ROM(s).  The default  method of  storing a ROM  set is to  place"
"         y sus ROM(s) cl?nicas.  La forma por defecto  de almacenar las  ROMs"
"         all duplicate  ROMs from the  parent and  clone ROMs into  a zip"
"         consiste en  colocar todas las  ROMs duplicadas en la original y sus"
"         file with the name  of the parent ROM and  place those ROMs that"
"         clones en un fichero zip  con el nombre de la ROM original,  y dejar"
"         are unique to a clone in a zip file with the name of that clone."
"         las ROMs propias  de cada clon  en un zip  con el  nombre del  clon."
"         However,  romalizer also  supports ROM sets  which have  all the"
"         Sin embargo,  romalizer tambi?n soporta conjuntos de ROMs que tengan"
"         parent ROMs and clone ROMs stored  in a single zip file with the"
"         almacenadas todas las  ROMs de la original  y los clones en  un solo"
"         name of the parent ROM.  In addition, romalizer  is also capable"
"         fichero zip con  el nombre de la ROM original.  Adem?s, romalizer es"
"         of building  ROM sets such  that each zip  file be it  parent or"
"         capaz de construir conjuntos de  ROMs que sean capaces de ejecutarse"
"         clone is  capable of running  on its own.  For more  information"
"         por  si  mismos,  ya  sea una  ROM original  o  un  clon.  Para  m?s"
"         on using  romalizer with  different ROM  set storing  techniques"
"         informaci?n sobre  c?mo utilizar  romalizer con  cada una  de  estas"
"         read about the -G, -g, and -z options."
"         t?cnicas de almacenamiento, vea las opciones -G, -g y -z."
"  OPTIONS"
"  OCIONES"
"         -c  level"
"         -c  nivel"
"                This specifies the compression level  you want to use for"
"                Especifica el nivel  de compresi?n  que quiere utilizar  para"
"                your  zip files.  0 indicates  no compression,  1 is  the"
"                los archivos zip.  0 indica sin compresi?n,  1 es el nivel de"
"                fastest compression,  and 9 is the  slowest.  The default"
"                compresi?n  m?s r?pido,  y 9 es el  m?s lento.  El nivel  por"
"                is 6."
"                defecto es el 6."
"         -d  workdir"
"         -d  dir_trabajo"
"                This  specifies  the  directory  where  romalizer  places"
"                Especifica  el   directorio  que  romalizer   utilizar?  para"
"                temporary  working files, and backs up your old ROMs when"
"                trabajar con  ficheros  temporales,  y para salvaguardar  sus"
"                building new  ones if the -M option is used.  The default,"
"                ROMs viejas cuando  est? construyendo  nuevos archivos si  se"
"                if not specified, is set to /tmp/romalizer."
"                us? la opci?n -M. Por defecto, usar? /tmp/romalizer."
"         -l  logfile"
"         -l  bit?cora"
"                This will send  output which is sent to  the console when"
"                Enviar? toda la salida que normalmente va a la consola cuando"
"                the -v option is used to a file specified by logfile.  If"
"                se indica la opci?n  -v al fichero  especificado.  Si se usan"
"                both the -l and -v options are used, then  output will be"
"                tanto la opci?n  -l como -v,  entonces la salida  se  enviar?"
"                sent to both the console and the logfile specified."
"                a la consola y al fichero de bit?cora especificado."
"         -r  rompath"
"         -r  dir_roms"
"                Specifies  the rompath  to use when looking  for your zip"
"                Especifica  el camino  o caminos en  los que buscar  archivos"
"                files.  If  not  specified,  romalizer  uses the  rompath"
"                zip con  las ROMs.  Si no se especifica,  romalizer utilizar?"
"                specified in your ~/.xmamerc file."
"                el que est? configurado en su fichero ~/.xmamerc."
"         -x  xmame"
"         -x  xmame"
"                Specifies the location of the xmame executable to use for"
"                Especifica el  camino  hasta el  ejecutable del xmame  que se"
"                checking  CRC values.  The default,  if not specified, is"
"                usar? para comprobar los CRCs.  Por defecto, si no se indica,"
"                xmame.x11."
"                es xmame.x11."
"         -C  cutoff"
"         -C  m?nimo"
"                Specifies the  cutoff value for the  number of ROMs which"
"                Especifica  el m?nimo  n?mero de ROMs  indicadas en  l?nea de"
"                have to be specified on the command line before romalizer"
"                comandos a  partir  del  cu?l romalizer  comprobar? sus  CRCs"
"                checks all CRCs at once instead of one ROM at a time. The"
"                todos  a la vez en lugar de  comprobarlos de uno en uno.  Por"
"                default is 150."
"                defecto se necesitan 150 ROMs."
"         -f     Force  creation  of  new  ROMs.  With  this  option  set,"
"         -f     Fuerza  la creaci?n de  nuevos archivos  de ROMs.  Al activar"
"                romalizer  will  automatically  create  new  ROMs without"
"                esta   opci?n,  romalizer   autom?ticamente   crear?   nuevos"
"                prompting you.  This  option is  useful  if you  want  to"
"                archivos  sin  preguntarle.  Esta opci?n  es ?til  si  quiere"
"                automatically fix a  large number of ROMs, and you do not"
"                reparar  autom?ticamente  un  n?mero  elevado de  ROMs,  y no"
"                want to answer a lot of prompts. This option is also used"
"                quiere responder a todas las confirmaciones de creaci?n. Esta"
"                frequently with the -n option."
"                opci?n se usa frecuentemente acompa?ada de -n."
"         -g     Tells romalizer you want  to merge your clone ROMs into a"
"         -g     Configura romalizer para unir  todos los clones  de una misma"
"                single file  with  the name  of the clone's  parent  ROM, "
"                ROM  en un  ?nico archivo  con el nombre  de la ROM original,"
"                rather than one file for each clone."
"                en lugar de tener un archivo para cada clon."
"         -h     Prints out this descriptive help file."
"         -h     Muestra esta ayuda."
"         -m     Check file names as well as CRC values in your zip files."
"         -m     Comprobar los nombres de los ficheros en los archivos zip."
"         -n     Do not  execute commands,  but  instead just  print  what"
"         -n     No  ejecutar  ning?n  comando,  si no  mostrar  qu?  comandos"
"                commands romalizer would execute if the -n option was not"
"                ejecutar?a  romalizer si  no se  hubiera  indicado  la opci?n"
"                used."
"                -n."
"         -v     Verbose mode.  This  option  will  print  all  the  shell"
"         -v     Modo  detallado.  Con  esta opci?n,  romalizer ir?  mostrando"
"                commands it is trying to execute to stdout."
"                por stdout todos los comandos que trata de ejecutar."
"         -z     When used with the -g option,  romalizer will build empty"
"         -z     Cuando se usa junto con la opci?n -g, romalizer generar? zips"
"                zip files  for the clone roms.  This is useful for front-"
"                vac?os para  las  ROMs cl?nicas.  Esto  es ?til para  que los"
"                ends to easily  identify which clone ROMs you have.  This"
"                interfaces  gr?ficos puedan  identificar f?cilmente  qu? ROMs"
"                option has no effect if the -g option is not used."
"                tiene. Esta opci?n no tiene efecto si no se usa -g."
"         -G     Tells romalizer  that you want to have a  complete set of"
"         -G     Indica a romalizer  que usted quiere tener un  juego completo"
"                ROMs in each zip file so that a clone may run independent"
"                de  ROMs  en  cada  zip,  de forma  que cada  clon pueda  ser"
"                of the parent zip file."
"                ejecutado independientemente sin su ROM original."
"         -M     Move fixed  ROMs into  ROM path and  backup the  old ROMs"
"         -M     Mover las ROMs reparadas al directorio de ROMs y salvaguardar"
"                into the working directory."
"                las viejas en el directorio de trabajo."
"         -V     Prints version number and exits."
"         -V     Mostrar el n?mero de versi?n y salir."
"  EXAMPLES"
"  EJEMPLOS"
"         To verify all your ROMs that start with the letter `a' use:"
"         Para verificar todas las ROMs que comiencen con la letra 'a', use:"
"                romalizer /usr/local/share/xmame/roms/a*"
"                romalizer /usr/local/share/xmame/roms/a*"
"         To see what romalizer would do without doing anything use:"
"         Para ver qu? har?a romalizer sin hacer realmente nada, use:"
"                romalizer -v -n pacman"
"                romalizer -v -n pacman"
"         To automatically fix any ROMs that romalizer sees problems with:"
"         Para reparar autom?ticamente cualquier ROM con problemas:"
"                romalizer -f pacman galaga joust"
"                romalizer -f pacman galaga joust"
"         Here is an example I use when a new version of xmame comes out:"
"         He aqu? un ejemplo que yo uso con cada versi?n nueva de xmame:"
"                romalizer -f -v -n -x ~/xmame.x11 -r ~/old:~/new ~/new/roms/*"
"                romalizer -f -v -n -x ~/xmame.x11 -r ~/old:~/new ~/new/roms/*"
"         My  ~/old/roms  directory contains my old xmame ROMs, and the"
"         Mi directorio ~/old/roms contiene las ROMs viejas del xmame, y el"
"         ~/new/roms directory contains the new ROMs"
"         directorio ~/new/roms las nuevas."
"         In  your directory  of fixed ROMs,  you can use  this command to"
"         En el directorio de ROMs reparadas, puede usar este comando para"
"         verify that romalizer has worked properly:"
"         verificar que romalizer funcion? correctamente:"
"                for i in *;do xmame -verifyroms `basename $i .zip`>>outf;done"
"                for i in *;do xmame -verifyroms `basename $i .zip`>>outf;done"
"         Then, you can examine the file 'outf' for any errors. "
"         Y puede examinar el fichero 'outf' para buscar cualquier error"
"  ENVIRONMENT"
"  ENTORNO"
"         MAMELANG"
"         MAMELANG"
"                Specify the translation file to use if you are running"
"                Indica el  fichero de  traducci?n a  utilizar si  ejecuta  el"
"                this script with a modified version of Tcl that supports"
"                programa  con  una  versi?n  modificada  de  Tcl que  soporte"
"                MAMELANG.  (See: http://www.emuhq.com/mamelang/)"
"                MAMELANG.  (Vea: http://www.emuhq.com/mamelang/)"
"         ROMPATH"
"         ROMPATH"
"                Overrides the  rompath specified  in the ~/.xmamerc file."
"                Sobreescribe  el camino  de ROMs  especificado  en el fichero"
"                If a rompath is  specified on the command line, then that"
"                ~/.xmamerc.  Si se  indica otro camino en l?nea  de comandos,"
"                is used instead of the environment setting."
"                entonces se usar? ese en lugar del de la variable de entorno."
"         PATH"
"         PATH"
"                Used to locate zip, unzip, and xmame commands."
"                Se utiliza para localizar los comandos zip, unzip y xmame."
"  SEE ALSO"
"  V?ASE TAMBI?N"
"  AUTHOR"
"  AUTOR"
"         romalizer  is written by  Christopher Stone  <tkmame@ztnet.com>."
"         romalizer ha sido escrito por Christopher Stone <tkmame@ztnet.com>."
"         If you have any ideas or  suggestions on improving this software"
"         Si tiene alguna  idea o sugerencia para  mejorar este programa, por"
"         please let me know."
"         favor h?gamelo saber."
"  BUGS"
"  PROBLEMAS"
"         NeoGeo BIOS ROMs are assumed to be in neogeo.zip and will not be"
"         Las ROMs de la  BIOS de NeoGeo se  asume que est?n  en neogeo.zip y"
"         included in NeoGeo zip files when using the -G option."
"         no se inluir?n en los archivos de ROMs NeoGeo al usar la opci?n -G."
"version"
"versi?n"
"Missing ROM!"
"?ROM no encontrada!"
"Try"
"Pruebe"
"for more options."
"para m?s opciones."
"Invalid compression level specified"
"Especificado nivel de compresi?n incorrecto"
"Unable to locate zip and/or unzip commands."
"No se pudo encontrar los comandos zip y/o unzip."
"Unable to execute command"
"No se pudo ejecutar el comando"
"Please send a bug report to"
"Por favor env?e un informe sobre el error a"
"Please upgrade to UnZip version 5.41 or greater"
"Por favor actualice su sistema con UnZip versi?n 5.41 o superior"
"Obtaining clone information"
"Obteniendo informaci?n de clones\t"
"Done"
"Hecho"
"Obtaining list of ROMs"
"Obteniendo lista de ROMs\t"
"No valid ROMs specified."
"No se especificaron ROMs v?lidas."
"Obtaining ROM path"
"Obteniendo camino de b?squeda de ROMs"
"Obtaining game descriptions"
"Obteniendo descripciones de los juegos\t"
"Obtaining CRC information"
"Obteniendo informaci?n de CRC\t\t"
"Unable to find"
"No se pudo encontrar"
"skipping."
"ignor?ndolo."
"Obtaining zipfile information"
"Obteniendo informaci?n del archivo zip"
"GOOD"
"BIEN"
"BAD"
"MAL"
"zip name"
"el nombre del zip"
"does not match"
"no coincide"
"The following files are missing in"
"Faltan los siguientes ficheros en"
"Found in"
"Encontrado(s) en"
"Not found in any other zips!"
"?No encontrado(s) en ning?n otro zip!"
"The following extranous files were found in"
"Se encontraron los siguientes ficheros innecesarios"
"Would you like me to fix your ROMs"
"?Quiere que repare sus ROMs"
"fixing"
"reparando"
"Your old ROMs have been moved to"
"Sus ROMs viejas han sido movidas a"
"The fixed ROMs have been placed in"
"Las ROMs reparadas han sido colocadas en"
