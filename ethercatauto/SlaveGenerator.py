#Test bench
from SlaveReader import SlaveReader
from SlaveReader import SlaveInfoReader
from ODGenerator import ODGenerator
from PDOMapping  import GenerateRxPdoMappingProg
from PDOMapping  import GenerateTxPdoMappingProg
from XmlGenerator import XmlGenerator

listSlaveDict = SlaveReader("SSC-Device.xls")
dictSlaveInfo = SlaveInfoReader("SSC-Device.xls")
strApplicationObjDic = ODGenerator(listSlaveDict)
strOutputMapping = GenerateRxPdoMappingProg(listSlaveDict)
strInputMapping = GenerateTxPdoMappingProg(listSlaveDict)

f = open("ethercat_template.txt")
strFile = f.read()
f.close()
dictMapping = {\
    "ApplicationObjDic"     :strApplicationObjDic,
    "OutputMapping"         :strOutputMapping,
    "InputMapping"          :strInputMapping,
    }
dictMapping.update(dictSlaveInfo)
strFile = strFile%dictMapping
        
f = open("ethercat.ino",'w')
f.write(strFile)
f.close()

strXmlFile = "SlaveDevice.xml"
XmlGenerator(strXmlFile,dictSlaveInfo,listSlaveDict)
