This file contains the full list of all libraries used by **Elypso Browser**. Lost Empire Entertainment does not own, did not create, does not actively develop or contribute to these libraries. All rights belong to the library developers and all questions and problems related to these libraries should be forwarded to their developers and not to Lost Empire Entertainment.

- CHANGES.txt - explains any changes made to this library compared to the developers' original format
- LICENSE.txt - a copy of the license provided by the developers of the library

Note: Modifying the structure or location of any of the external library files is highly discouraged as this will break things. Modify at your own risk.

### Full List of Libraries:

#### Core Components
- **GLFW** - Window management
- **GLM** - Math library (for transformations, vectors, etc.)
- **Glad** - OpenGL function loader
- **ImGui** - Debug UI

#### Web Rendering
- **LiteHTML** - HTML & CSS parsing/rendering
- **QuickJS** - JavaScript engine

#### Networking & Security
- **LibCurl** - HTTP(S) networking
- **NGHTTP2** - HTTP/2 support (optional, but useful for modern sites)
- **OpenSSL** - TLS/SSL for secure connections
- **LibWebSockets** - WebSockets support for real-time web communication
- **LibDataChannel** - WebRTC implementation for peer-to-peer communication

#### Fonts & Images
- **FreeType** - Font rendering
- **HarfBuzz** - Advanced text shaping (ligatures, kerning, complex scripts)
- **stb_image** - Image loading (for inline images, favicons, etc.)

#### Storage & Cookies
- **LibPSL** - Persistent storage for cookies
- **SQLite** - Persistent storage for cookies, history, cache, and IndexedDB-like functionality

