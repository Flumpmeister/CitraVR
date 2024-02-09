#include "../OpenXR.h"
#include "../Swapchain.h"
#include "../utils/Common.h"

#include <string>

class UILayer {

public:

    /** Constructor.
     * @param position: position of the layer, in world space
     * @param activity object: reference to the current activity. Used to get
     * the class information for gameSurfaceClass
     * @param session a valid XrSession
     */
    UILayer(const std::string& className, const XrVector3f&& position, JNIEnv* jni, jobject activityObject,
                     const XrSession& session);
    ~UILayer();

    /** Called on resume. Sets the surface in the native rendering library.
     * Overrides the normal surface passed by Citra
     */
    void SetSurface() const;

    /** Called once-per-frame. Populates the given layer descriptor to show the
     *  top and bottom panels as two separate layers.
     *
     *  @param space the XrSpace this layer should be positioned with. The
     * center of the layer is placed in the center of the FOV.
     *  @param layers the array of layers to populate
     *  @param layerCount the number of layers in the array
     */
    void Frame(const XrSpace& space, std::vector<XrCompositionLayer>& layers,
               uint32_t& layerCount) const;

    /** Given an origin, direction of a ray,
     *  returns the coordinates of where the ray will intersects
     *  with the UI layer. This is used to render the controller cursor
     *  and send clicks to the Android window coordinates.
     *
     *  @param start origin of the ray
     *  @param end the destination point of the ray
     *  @param result2d stores the 2D result on success. The 2D result is the 2D
     *   location of the intersection (cursor) in the Android display coordinate
     *   system. This is used to send input events to the window.
     *  @param result3d store the 3D result on success. The 3d result is the
     *   position where the layer and the ray intersect, in the target reference
     *   space. This is used to render the cursor.
     *
     *  @return true if the ray and the plane intersected and the position is
     *   within the layer's window bounds, false otherwise.
     *
     * Note: assumes viewer is looking down the -Z axis.
     */
    bool GetRayIntersectionWithPanel(const XrVector3f& start, const XrVector3f& end,
                                     XrVector2f& result2d, XrPosef& result3d) const;

    void SendClickToUI(const XrVector2f& pos2d, const int type);
private:
    int Init(const std::string& className, const jobject activityObject, const XrVector3f& position, const XrSession& session);
    void Shutdown();

    /** Creates the swapchain.
     */
    void CreateSwapchain();

    const XrSession session_;
    Swapchain swapchain_;

    XrPosef panelFromWorld_;

    //============================
    // JNI objects
    JNIEnv* env_ = nullptr;
    jobject activityObject_ = nullptr;
    jclass vrUILayerClass_ = nullptr;
    jobject vrUILayerObject_ = nullptr;
    jobject surface_ = nullptr;

    //============================
    // JNI methods
    jmethodID getBoundsMethodID_         = nullptr;
    jmethodID sendClickToWindowMethodID_ = nullptr;
};
