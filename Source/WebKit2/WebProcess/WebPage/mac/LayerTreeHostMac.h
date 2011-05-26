/*
 * Copyright (C) 2011 Apple Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. AND ITS CONTRIBUTORS ``AS IS''
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL APPLE INC. OR ITS CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef LayerTreeHostMac_h
#define LayerTreeHostMac_h

#include "LayerTreeContext.h"
#include "LayerTreeHost.h"
#include <WebCore/GraphicsLayerClient.h>
#include <wtf/OwnPtr.h>
#include <wtf/RetainPtr.h>

typedef struct __WKCARemoteLayerClientRef* WKCARemoteLayerClientRef;

namespace WebKit {

class LayerTreeHostMac : public LayerTreeHost, WebCore::GraphicsLayerClient {
public:
    static PassRefPtr<LayerTreeHostMac> create(WebPage*);
    ~LayerTreeHostMac();
    
private:
    explicit LayerTreeHostMac(WebPage*);

    // LayerTreeHost.
    virtual const LayerTreeContext& layerTreeContext();
    virtual void scheduleLayerFlush();
    virtual void setRootCompositingLayer(WebCore::GraphicsLayer*);
    virtual void invalidate();

    virtual void setNonCompositedContentsNeedDisplay(const WebCore::IntRect&);
    virtual void scrollNonCompositedContents(const WebCore::IntRect& scrollRect, const WebCore::IntSize& scrollOffset);
    virtual void sizeDidChange(const WebCore::IntSize& newSize);

    virtual void didInstallPageOverlay();
    virtual void didUninstallPageOverlay();
    virtual void setPageOverlayNeedsDisplay(const WebCore::IntRect&);

    // GraphicsLayerClient
    virtual void notifyAnimationStarted(const WebCore::GraphicsLayer*, double time);
    virtual void notifySyncRequired(const WebCore::GraphicsLayer*);
    virtual void paintContents(const WebCore::GraphicsLayer*, WebCore::GraphicsContext&, WebCore::GraphicsLayerPaintingPhase, const WebCore::IntRect& clipRect);
    virtual bool showDebugBorders() const;
    virtual bool showRepaintCounter() const;

    static void flushPendingLayerChangesRunLoopObserverCallback(CFRunLoopObserverRef, CFRunLoopActivity, void*);
    void flushPendingLayerChangesRunLoopObserverCallback();
    bool flushPendingLayerChanges();

    void createPageOverlayLayer();
    void destroyPageOverlayLayer();

    // The context for this layer tree.
    LayerTreeContext m_layerTreeContext;

    // Whether the layer tree host is valid or not.
    bool m_isValid;    

    // The root layer.
    OwnPtr<WebCore::GraphicsLayer> m_rootLayer;

    // The layer which contains all non-composited content.
    OwnPtr<WebCore::GraphicsLayer> m_nonCompositedContentLayer;

    // The page overlay layer. Will be null if there's no page overlay.
    OwnPtr<WebCore::GraphicsLayer> m_pageOverlayLayer;

    RetainPtr<WKCARemoteLayerClientRef> m_remoteLayerClient;
    RetainPtr<CFRunLoopObserverRef> m_flushPendingLayerChangesRunLoopObserver;
};

} // namespace WebKit

#endif // LayerTreeHostMac_h