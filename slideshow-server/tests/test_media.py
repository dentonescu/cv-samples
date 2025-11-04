from slideshow_server import (
    build_ffmpeg_command,
    discover_media_files
)

def test_build_ffmpeg_command_contains_target_path():
    target = "/tmp/video.mp4"
    command = build_ffmpeg_command(target)
    assert command[0] == "ffmpeg"
    assert target in command

def test_discover_media_files_filters_supported_extensions(tmp_path):
    (tmp_path / "keep.jpg").write_bytes(b"x")
    (tmp_path / "skip.txt").write_text("this should be skipped")
    nested = tmp_path / "nested"
    nested.mkdir()
    (nested / "clip.MP4").write_bytes(b"x")
    files = discover_media_files(tmp_path)
    assert set(files) == {"keep.jpg", "nested/clip.MP4"}