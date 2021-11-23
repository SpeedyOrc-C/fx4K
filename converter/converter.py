from json import loads


def get_path(prompt: str) -> str:
    path = input(prompt)
    if path[0] == '\'':
        path = path[1:]
    if path[-1] == '\'':
        path = path[:-1]
    return path


def str_32_bytes(s: str) -> bytes:  # 32 bytes
    if len(s) < 32:
        s += '\0' * (32 - len(s))
    elif len(s) > 32:
        s = s[:32]
    return s.encode('ascii')


def short_bytes(x: int) -> bytes:  # 2 bytes
    return bytes([(x >> 8) % 256, x % 256])


def char_bytes(x: int) -> bytes:  # 1 byte
    return bytes([x % 256])


def note_bytes(note: dict, bpm: int) -> bytes:

    is_hold = len(note.keys()) == 3
    beat: list[int]
    beat = note['beat']
    column = note['column']

    if not is_hold:
        return\
            short_bytes(round( (beat[0]+beat[1]/beat[2])*40*60/bpm )) +\
            short_bytes(round( (beat[0]+beat[1]/beat[2])*40*60/bpm )) +\
            bytes([ord('C')]) +\
            bytes([column]) +\
            bytes([0] * 2)

    else:
        endbeat = note['endbeat']
        return\
            short_bytes(round( (beat[0]+beat[1]/beat[2])*40*60/bpm )) +\
            short_bytes(round( (endbeat[0]+endbeat[1]/endbeat[2])*40*60/bpm )) +\
            bytes([ord('H')]) +\
            bytes([column]) +\
            bytes([0] * 2)


def main():

    try:
        with open(get_path(
            'Malody 铺面路径(可拖拽至此)\nMalody chart path (or drag and drop)\n'
            ), 'r') as malody_chart_raw:
            try:
                malody_chart = loads(malody_chart_raw.read())
                malody_title = malody_chart['meta']['song']['title']
                malody_artist = malody_chart['meta']['song']['artist']
                malody_bpm = round(malody_chart['time'][0]['bpm'])
                malody_notes = malody_chart['note']
                malody_note_count = len(malody_notes)
                duration = 0
                for note in malody_chart['note']:
                    try:
                        note['sound']
                        malody_note_count -= 1
                    except: pass
                print('标题 Title: %s\n艺人 Artist: %s\nBPM : %.2f\n物量 Note Count: %d' % (
                    malody_title, malody_artist, malody_bpm, malody_note_count))
            except:
                print('铺面已损坏 Chart is corrupted.')
                return
    except:
        print('文件不存在 File does not exist.')
        return

    with open(input(
        '输出文件名(最多八个字母)\nOutput file name (Maximum 8 Latin letters)\n'
        ), 'wb') as fx4k_chart_raw:
        try:
            fx4k_title = str_32_bytes(malody_title)
            fx4k_artist = str_32_bytes(malody_artist)
        except:
            print('铺面名称只能包含ASCII字符 Only ASCII character is allowed.')
            return
        fx4k_bpm = short_bytes(malody_bpm)
        fx4k_note_count = short_bytes(malody_note_count)

        fx4k_chart_raw.write(
            fx4k_title +
            fx4k_artist +
            fx4k_bpm +
            fx4k_note_count +
            bytes([0] * 12)  # Leave blank
        )

        for note in malody_notes:

            try:
                note['sound']
                continue
            except: pass

            beat = note['beat']
            duration = max(duration, round((beat[0]+beat[1]/beat[2])*40*60/malody_bpm))
            try:
                endbeat = note['beat']
                duration = max(duration, round((endbeat[0]+endbeat[1]/endbeat[2])*40*60/malody_bpm))
            except: pass

            note_raw = note_bytes(note, malody_bpm)
            fx4k_chart_raw.write(note_raw)

        print('时长 Duration: %d' % duration)
        fx4k_chart_raw.seek(0x44)
        fx4k_chart_raw.write(short_bytes(duration))

main()
