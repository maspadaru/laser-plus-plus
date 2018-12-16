from stream import Stream


class FileStream(Stream):
    def __init__(self, filename, nTriples, startTime, endTime):
        super(FileStream, self).__init__()
        self.tupleCounter = 0
        self.nTriples = nTriples
        self.startTime = startTime
        self.endTime = endTime
        self.stream_file = open(filename)
        self.timeLine = {
            "startTime": startTime,
            "endTime"  : endTime
        }

    def get_fact(self):
        line_list = self.stream_file.readline().split()
        if (len(line_list) > 1):
            predicate = line_list[0]
            groundings = line_list[1:]
            return predicate, groundings
        return None 

    def getNumberOfTuplesAt(self, t):
        return self.nTriples

    def getTimeLine(self):
        return self.timeLine

    def hasTimePoint(self, t):
        return t >= self.startTime and t <= self.endTime 

    def get(self, t):
        result = dict() 
        for i in range (0, self.nTriples):
            row = [t, self.timeLine["endTime"], self.tupleCounter, None]
            fact = self.get_fact()
            if fact is not None:
                predicate, constant_list = fact;
                row.extend(constant_list)
                if predicate not in result:
                    result[predicate] = set()
                result[predicate].add(tuple(row))
        return result 
