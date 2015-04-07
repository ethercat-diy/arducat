
def GeneratePdoMapping(listSlaveInfo,rxtx,strEntryPrefix,strPdoObjPrefix):
    from EthercatType import dictEthercatType, dictAccessType, dictPdoDirType
    #Get items start with 0x6xxx and with 'tx' attribute
    #Generate list {RxPdoObj: [(Index,SI,length,intBitOffset)]} with intBitOffset = 0 temporaly
    dictPdoMapping = {}
    for dictItem in listSlaveInfo:
        if len(dictItem['Index'])!=6 or dictItem['Index'][0:3]!=strEntryPrefix:
            continue
        strPdoMappingObj = strPdoObjPrefix+dictItem['Index'][3:5]
        if dictItem['ObjectCode'] == "VARIABLE":
            if dictItem['Entry'][0]['PdoDirection'] != rxtx:
                continue
            dictMappingEntry = {'Index':dictItem['Index'],'SI':0,'Length':dictEthercatType[dictItem['Entry'][0]['DataType']][2],'BitOffset':0}
            dictMappingEntry['Name'] = dictItem['Entry'][0]['Name']
            if dictMappingEntry['Name']=="":
                dictMappingEntry['Name'] =  "Obj"+dictItem['Index'][2:]
            dictMappingEntry['DataType'] = dictItem['Entry'][0]['DataType']
            if strPdoMappingObj in dictPdoMapping:
                dictPdoMapping[strPdoMappingObj].append(dictMappingEntry)
            else:
                dictPdoMapping[strPdoMappingObj] = [dictMappingEntry]
        if dictItem['ObjectCode'] == "RECORD":
            for dictEntry in dictItem['Entry']:
                if dictEntry['PdoDirection'] != rxtx:
                    continue
                dictMappingEntry = {'Index':dictItem['Index'],'SI':dictEntry['SI'],'Length':dictEthercatType[dictEntry['DataType']][2],'BitOffset':0}
                dictMappingEntry['Name'] = dictEntry['Name']
                if dictMappingEntry['Name']=="":
                    dictMappingEntry['Name'] =  "SubIndex "+"%03d"%dictEntry['SI']
                dictMappingEntry['DataType'] = dictEntry['DataType']
                if strPdoMappingObj in dictPdoMapping:
                    dictPdoMapping[strPdoMappingObj].append(dictMappingEntry)
                else:
                    dictPdoMapping[strPdoMappingObj] = [dictMappingEntry]
        if dictItem['ObjectCode'] == "ARRAY":
            if dictItem['Entry'][1]['PdoDirection'] != rxtx:
                continue
            for intSI in range(1,int(dictItem['Entry'][0]['Default'])+1):
                dictMappingEntry = {'Index':dictItem['Index'],'SI':intSI,'Length':dictEthercatType[dictItem['Entry'][1]['DataType']][2],'BitOffset':0}
                dictMappingEntry['Name'] = "SubIndex "+"%03d"%intSI
                dictMappingEntry['DataType'] = dictItem['Entry'][1]['DataType']
                if strPdoMappingObj in dictPdoMapping:
                    dictPdoMapping[strPdoMappingObj].append(dictMappingEntry)
                else:
                    dictPdoMapping[strPdoMappingObj] = [dictMappingEntry]
    #Sort
    dictPdoMapping = sorted(dictPdoMapping.iteritems(), key=lambda asd:asd[0], reverse = False)
    #Generate BitOffset and Dummy items
    dictPdoMappingPost = {}
    for itemPdoMapping in dictPdoMapping:
        strPdoMappingObj = itemPdoMapping[0]
        listMappingEntries = itemPdoMapping[1]
        dictPdoMappingPost[strPdoMappingObj] = []
        bitOffset = int(0)
        listMappingEntriesPost = []
        for dictMappingEntry in listMappingEntries:
            #Align PDO mapping to byte
            if bitOffset%8!=0 and dictMappingEntry['Length']<8 and (bitOffset%8+dictMappingEntry['Length']>8):
                dictDummyEntry = {'Index':"0x0000",'SI':0,'Length':8-(bitOffset%8),'BitOffset':bitOffset}
                dictPdoMappingPost[strPdoMappingObj].append(dictDummyEntry)
                bitOffset = (bitOffset/8+1)*8
            #Add to bit Offset
            dictMappingEntry['BitOffset'] = bitOffset
            dictPdoMappingPost[strPdoMappingObj].append(dictMappingEntry)
            bitOffset = bitOffset+dictMappingEntry['Length']
    dictPdoMapping = dictPdoMappingPost
    return dictPdoMapping
	
	
def GenerateTxPdoMapping(listSlaveInfo):
    return GeneratePdoMapping(listSlaveInfo,"tx","0x6","0x1A")

def GenerateRxPdoMapping(listSlaveInfo):
    return GeneratePdoMapping(listSlaveInfo,"rx","0x7","0x16")

def GeneratePdoMappingProg(listSlaveInfo,rxtx):
    if rxtx=="rx":
        dictPdoMapping = GenerateRxPdoMapping(listSlaveInfo)
        cmdBytesCopy = '\t\t\t\t*((UINT8*)&(%(strVarName)s)+%(intByte)d)'
        cmdBytesCopy = cmdBytesCopy+' = *(pcData+%(ByteOffset)d);\n'
        cmdMskShift = '\t\t\t\t*((UINT8*)&(%(strVarName)s))'
        cmdMskShift = cmdMskShift+' = (*(pcData+%(ByteOffset)d)&0x%(intMask)x)'
        cmdMskShift = cmdMskShift+'>>(%(intOffset)d);\n'
    elif rxtx=="tx":
        dictPdoMapping = GenerateTxPdoMapping(listSlaveInfo)
        cmdBytesCopy = '\t\t\t\t*(pcData+%(ByteOffset)d)'
        cmdBytesCopy = cmdBytesCopy+' = *((UINT8*)&(%(strVarName)s)+%(intByte)d);\n'
        cmdMskShift = '\t\t\t\t*(pcData+%(ByteOffset)d) &= ~0x%(intMask)x;\n'
        cmdMskShift = cmdMskShift+'\t\t\t\t*(pcData+%(ByteOffset)d) |= '
        cmdMskShift = cmdMskShift+'(*((UINT8*)&(%(strVarName)s))<<(%(intOffset)d))'
        cmdMskShift = cmdMskShift+'&0x%(intMask)x;\n'
    strResult = ''
    for itemPdoMapping in dictPdoMapping.iteritems():
        bitEnd = int(0)
        strResult = strResult+"\t\t\tcase "+itemPdoMapping[0]+':\n'
        for dictEntry in itemPdoMapping[1]:
            #Skip dummy objects
            if dictEntry['Index'] == "0x0000":
                continue;
            #Get destination variable
            if dictEntry['SI'] == 0:
                strVarName = "Obj"+dictEntry['Index']
            else:
                strVarName = "Obj"+dictEntry['Index']+".SubIndex"\
                             +str(dictEntry['SI'])
            #Copy bytes variables
            if dictEntry['Length'] >= 8:
                for intByte in range(0,dictEntry['Length']/8):
                    strResult = strResult+cmdBytesCopy%{'strVarName':strVarName,\
                                              'intByte':intByte,\
                                              'ByteOffset':dictEntry['BitOffset']/8+intByte}
            #Copy bits variables
            else:
                intMask = int(2**(dictEntry['Length'])-1)<<(dictEntry['BitOffset']%8)
                strResult = strResult+cmdMskShift%{'strVarName':strVarName,\
                                                   'intMask':intMask,\
                                                   'intOffset':dictEntry['BitOffset']%8,\
                                                   'ByteOffset':dictEntry['BitOffset']/8}
            bitEnd = dictEntry['BitOffset']+dictEntry['Length']
        strResult = strResult+'\t\t\t\tpcData += '+str((bitEnd+7)/8)+';\n'
        strResult = strResult+'\t\t\t\tbreak;\n'
    return strResult   

def GenerateRxPdoMappingProg(listSlaveInfo):
    return GeneratePdoMappingProg(listSlaveInfo,"rx")

def GenerateTxPdoMappingProg(listSlaveInfo):
    return GeneratePdoMappingProg(listSlaveInfo,"tx")
