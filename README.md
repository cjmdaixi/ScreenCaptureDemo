# ScreenCaptureDemo

Screen recording to mp4 on macOS using Qt & ffmpeg.

The screenshots are taken by `QScreen`'s `grabWindow` method at 30fps. Then the screenshots are sent to ffmpeg via zmq protocol, where the images are encoded to mp4.

The standalone ffmpeg is started using the start_ffmpeg.sh. In the shell script, the hardware acceleration is used on an Apple Sillicon chip.