import json


def popFromInput(input: str) -> tuple[str, str]:
    assert input.startswith("|")
    try:
        nextStart: int = input[1:].index("|") + 1
        return input[1:nextStart], input[nextStart:]
    except ValueError:
        return input[1:], ""


def getRoomInfo(input: str) -> tuple[str, str]:
    assert input.startswith(">")
    try:
        endIndex: int = input.index("|")
        return input[1:endIndex].strip().rstrip(), input[endIndex:].strip().rstrip()
    except ValueError:
        return input[1:].strip().rstrip(), ""


def parseSocketInput(input: str):
    nextArg, rest = popFromInput(input)
    match nextArg:
        case "updateuser":
            return
        case "formats":
            return
        case "challstr":
            return
        case "queryresponse":
            queryType, rest = popFromInput(rest)
            match queryType:
                case "roomlist":
                    roomlist, _ = popFromInput(rest)
                    roomlist_json = json.loads(roomlist)
                    return roomlist_json
                case _:
                    return


def filterRooms(rooms: dict[str, dict], targetPlayers: list[str]) -> dict[str, dict]:
    filteredRooms = {}
    for room in rooms:
        if rooms[room]["p1"].lower() in targetPlayers or rooms[room]["p2"].lower() in targetPlayers:
            filteredRooms[room] = rooms[room]
    return filteredRooms


async def saveLog(roomId: str, log: str):
    with open(f"logs/{roomId}.txt", "w", encoding='utf-8') as f:
        f.write(log)
