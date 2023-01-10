#include <catch2/catch.hpp>
#include <../scoreboardocr/src/capturemanager.h>

TEST_CASE("Capture manager - change current index", "[CaptureManager]")
{
    CaptureManager cap;
    SECTION("Change index to negative value")
    {
        cap.changeCurrentDeviceIndex(-1);
        REQUIRE(cap.flags.testFlag(CaptureManager::captureSelected) == false);
    }
    SECTION("Change index to positive value")
    {
        cap.changeCurrentDeviceIndex(1);
        REQUIRE(cap.flags.testFlag(CaptureManager::captureSelected) == true);
    }
}

TEST_CASE("Capture manager - get capture devices (at least one camera must be plugged in!)", "[CaptureManager]")
{
    CaptureManager cap;
    QList<Camera> cams = cap.getDevices();
    REQUIRE(cams.size() > 0);
    REQUIRE(cams[0].index >= 0);
    REQUIRE(!cams[0].name.isEmpty());
    REQUIRE(!cams[0].description.isEmpty());
}

TEST_CASE("Capture manager - initiate capture", "[CaptureManager]")
{
    CaptureManager cap;
    SECTION("Capture device index is not positive")
    {
        REQUIRE(cap.initCapture() == -1);
        REQUIRE(cap.flags.testFlag(CaptureManager::captureStarted) == false);
    }
    SECTION("Capture device index is positive, but devices not found")
    {
        cap.changeCurrentDeviceIndex(0);
        REQUIRE(cap.initCapture() == -1);
        REQUIRE(cap.flags.testFlag(CaptureManager::captureStarted) == false);
    }
    SECTION("Capture device index is positive, devices are found")
    {
        QList<Camera> cams = cap.getDevices();
        cap.changeCurrentDeviceIndex(cams[0].index);
        REQUIRE(cap.initCapture() == 1);
        REQUIRE(cap.flags.testFlag(CaptureManager::captureStarted) == true);
    }
    cap.stopCapture();
}

TEST_CASE("Capture manager - set edges", "[CaptureManager]")
{
    CaptureManager cap;
    QList<QPoint> points, res;

    points.append(QPoint(100,3));
    points.append(QPoint(4,104));
    points.append(QPoint(0,0));
    points.append(QPoint(108,109));

    cap.setEdges(points);
    res = *cap.getEdges();

    res[0] = QPoint(0, 0);
    res[1] = QPoint(100, 3);
    res[2] = QPoint(4, 104);
    res[3] = QPoint(108, 109);

    REQUIRE(cap.flags.testFlag(CaptureManager::edgesMarked) == true);
    REQUIRE(cap.flags.testFlag(CaptureManager::capturingEdges) == false);
}

TEST_CASE("Capture manager - capture frame (camera must be connected!)", "[CaptureManager]")
{
    CaptureManager cap;
    QList<Camera> cams = cap.getDevices();
    cap.changeCurrentDeviceIndex(cams[0].index);
    cap.initCapture();
    REQUIRE(cap.captureFrame());
}
