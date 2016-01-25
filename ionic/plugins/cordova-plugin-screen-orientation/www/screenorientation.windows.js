var screenOrientation = {};

screenOrientation.setOrientation = function (orientation) {
    var orientationNumber;
    switch (orientation) {
        case 'landscape':
            orientationNumber = 5;
            break;
        case 'portrait':
            orientationNumber = 10;
            break;
        case 'landscape-primary':
            orientationNumber = 1;
            break;
        case 'landscape-secondary':
            orientationNumber = 4;
            break;
        case 'portrait-primary':
            orientationNumber = 2;
            break;
        case 'portrait-secondary':
            orientationNumber = 8;
            break;
        case 'unlocked':
            orientationNumber = 0;
            break;
        default:
            break;
    }
    Windows.Graphics.Display.DisplayInformation.autoRotationPreferences = orientationNumber;
};

module.exports = screenOrientation;
