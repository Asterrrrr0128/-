import wave
import math

# 定義音符對應的頻率
def pitch_frequency(tone):
    frequencies = {
        'C3': 130.8, 'E3': 164.8, 'G3': 196.0,  # C 大調和弦
        'G2': 98.0, 'B2': 123.5, 'D3': 146.8,  # G 和弦
        'A2': 110.0, 'C3': 130.8, 'E3': 164.8,  # Am 和弦
        'F2': 87.3, 'A2': 110.0, 'C3': 130.8,  # F 和弦
        'C4': 261.6, 'E4': 329.6, 'G4': 392.0,  # 高音旋律
    }
    return frequencies.get(tone, 0)  # 無效音符回傳 0（休止符）

# 播放單一音符組合
def write_tone(wav_file, freq1, freq2, duration, sample_rate, amplitude):
    for i in range(int(sample_rate * duration)):
        sample1 = int(amplitude * math.sin(2 * math.pi * freq1 * i / sample_rate))
        sample2 = int(amplitude * math.sin(2 * math.pi * freq2 * i / sample_rate))
        wav_file.writeframes(sample1.to_bytes(2, 'little', signed=True))
        wav_file.writeframes(sample2.to_bytes(2, 'little', signed=True))

# 定義音樂段落
def play_section(wav_file, section, sample_rate, amplitude):
    for note1, note2, duration in section:
        freq1 = pitch_frequency(note1)
        freq2 = pitch_frequency(note2)
        write_tone(wav_file, freq1, freq2, duration, sample_rate, amplitude)

# WAV 文件參數
num_channels = 2  # 立體聲
sample_width = 2  # 16位元
sample_rate = 44100  # 取樣率
amplitude = 32767  # 最大振幅

# 創建 WAV 文件
with wave.open("new_composition.wav", 'w') as wav_file:
    wav_file.setnchannels(num_channels)
    wav_file.setsampwidth(sample_width)
    wav_file.setframerate(sample_rate)

    # 新的編曲 (四小節)
    section1 = [
        # 第一小節 - C 和弦 + 高音
        ('C3', 'E4', 0.5), ('G3', 'G4', 0.5),
        # 第二小節 - G 和弦 + 高音
        ('G2', 'D3', 0.5), ('B2', 'G4', 0.5),
        # 第三小節 - Am 和弦 + 高音
        ('A2', 'C4', 0.5), ('E3', 'G4', 0.5),
        # 第四小節 - F 和弦 + 高音
        ('F2', 'A2', 0.5), ('C3', 'E4', 0.5),
    ]
    
    # 播放段落
    play_section(wav_file, section1, sample_rate, amplitude)

print("新的編曲已輸出至 new_composition.wav")
