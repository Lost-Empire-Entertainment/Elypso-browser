# Third-Party Libraries Used in Elypso Browser

Lost Empire Entertainment does not own, did not create, does not actively develop, or contribute to these libraries. All rights belong to their respective developers. Any questions or issues regarding these libraries should be directed to their maintainers, not to Lost Empire Entertainment.

- **CHANGES.txt** - Explains any modifications made to this library compared to the original version.
- **LICENSE.txt** - A copy of the license provided by the library’s developers.

**Important:** Modifying the structure or location of any external library files is highly discouraged, as it may break functionality. Modify at your own risk.

---

## Core Components
| Library         | License         | Purpose |
|----------------|----------------|---------|
| **GLFW**       | zlib/libpng     | Window and input management |
| **GLM**        | MIT             | Math library for graphics and physics |
| **Glad**       | MIT             | OpenGL function loader |
| **khrplatform.h (in Glad folder)** | Apache 2.0 | OpenGL platform definitions |
| **ImGui**      | MIT             | Immediate Mode GUI for core browser GUI |

## Fonts & Text Rendering
| Library        | License                  | Purpose |
|---------------|--------------------------|---------|
| **FreeType**  | FreeType License (FTL)   | Font rendering |
| **HarfBuzz**  | MIT                      | Text shaping and rendering |
| **stb_image** | Public Domain/MIT        | Image loading library |

## Web Rendering & Processing
| Library         | License     | Purpose |
|----------------|------------|---------|
| **Lexbor**     | Apache 2.0  | HTML parsing and rendering |
| none           | -           | JavaScript engine |

## Networking & Communication
| Library           | License      | Purpose |
|------------------|-------------|---------|
| **libcurl**      | MIT         | HTTP(S) networking |
| **nghttp2**      | MIT         | HTTP/2 support |
| **LibWebSockets**| MIT         | WebSockets support |
| **LibDataChannel** | MPL-2.0   | WebRTC peer-to-peer communication |
| **OpenSSL**      | Apache 2.0  | TLS/SSL encryption |
| **Brotli**       | MIT         | HTTP content compression/decompression |
| **Zstd**         | BSD         | Advanced data compression (Zstandard) |

## Storage & Data Management
| Library     | License       | Purpose |
|------------|--------------|---------|
| **SQLite** | Public Domain | Database storage for cookies and history |
| **libpsl** | MIT          | Public Suffix List handling for domains |

Each library’s **license file** is included in its respective directory.
