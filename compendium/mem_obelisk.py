import plotly.graph_objects as go
import numpy as np


def create_block(x, y, z, dx, dy, dz, color, name, info):
    """Crée un bloc 3D représentant une section mémoire"""
    return go.Mesh3d(
        x=[x, x, x + dx, x + dx, x, x, x + dx, x + dx],
        y=[y, y + dy, y, y + dy, y, y + dy, y, y + dy],
        z=[z, z, z, z, z + dz, z + dz, z + dz, z + dz],
        i=[7, 0, 0, 0, 4, 4, 6, 6, 4, 0, 3, 2],
        j=[3, 4, 1, 2, 5, 6, 5, 2, 0, 1, 6, 3],
        k=[0, 7, 2, 3, 6, 7, 1, 1, 5, 5, 7, 6],
        color=color,
        opacity=0.8,
        name=name,
        hoverinfo="text",
        text=f"<b>{name}</b><br>{info}",
        showscale=False,
    )


def create_wireframe(x, y, z, dx, dy, dz):
    """Crée les contours pour l'effet 'Obélisque de verre'"""
    # Lignes verticales
    xe = [x, x, x + dx, x + dx, x] + [x, x, x + dx, x + dx, x] + [x] * 5
    ye = [y, y + dy, y + dy, y, y] + [y, y + dy, y + dy, y, y] + [y] * 5
    ze = (
        [z, z, z, z, z]
        + [z + dz, z + dz, z + dz, z + dz, z + dz]
        + [z, z + dz, z, z + dz, z]
    )

    # Correction simple pour le wireframe (juste les coins principaux)
    x_lines = [x, x, x + dx, x + dx, x, x, x + dx, x + dx]
    y_lines = [y, y + dy, y, y + dy, y, y + dy, y, y + dy]
    z_lines = [z, z, z, z, z + dz, z + dz, z + dz, z + dz]

    return go.Scatter3d(
        x=x_lines,
        y=y_lines,
        z=z_lines,
        mode="markers",
        marker=dict(size=2, color="white", opacity=0.3),
        showlegend=False,
        hoverinfo="none",
    )


# Configuration de la scène
fig = go.Figure()

# 1. KERNEL SPACE (Haut - Interdit)
fig.add_trace(
    create_block(
        0,
        0,
        900,
        10,
        10,
        100,
        "#2c3e50",
        "KERNEL SPACE",
        "Ring 0<br>Accès interdit utilisateur",
    )
)

# 2. STACK (Pile - Crépusculaire, grandit vers le bas)
fig.add_trace(
    create_block(
        1,
        1,
        750,
        8,
        8,
        50,
        "#8e44ad",
        "STACK",
        "RW-<br>Variables Locales<br>Frames de fonctions<br>Vuln: Buffer Overflow",
    )
)

# 3. ESPACE LIBRE (ASLR Gap)
# Invisible ou translucide

# 4. HEAP (Tas - Le chantier chaotique)
# Chunk Alloué 1
fig.add_trace(
    create_block(
        1,
        1,
        300,
        8,
        8,
        20,
        "#e67e22",
        "HEAP - Chunk A",
        "Allocated<br>Size: 0x20<br>Data: User Input",
    )
)
# Chunk Alloué 2 (Cible)
fig.add_trace(
    create_block(
        1,
        1,
        325,
        8,
        8,
        20,
        "#d35400",
        "HEAP - Chunk B",
        "Allocated<br>Contient Pointeur de Fct<br>Cible écrasement",
    )
)
# Chunk "Free" (Vulnérable)
fig.add_trace(
    create_block(
        1,
        1,
        350,
        8,
        8,
        15,
        "#c0392b",
        "HEAP - Free Chunk",
        "State: Freed<br>Vuln: Use-After-Free<br>Double Free",
    )
)

# 5. DATA / BSS
fig.add_trace(
    create_block(
        1,
        1,
        200,
        8,
        8,
        30,
        "#f1c40f",
        ".DATA / .BSS",
        "RW-<br>Globales & Static<br>Cible si NX bit actif",
    )
)

# 6. TEXT / RODATA (Fondations)
fig.add_trace(
    create_block(
        1,
        1,
        50,
        8,
        8,
        100,
        "#2980b9",
        ".TEXT / .RODATA",
        "R-X / R--<br>Code Assembleur & Constantes<br>Immuable (Segfault si écriture)",
    )
)

# Mise en forme "Cyberpunk"
fig.update_layout(
    title="Architecture Mémoire Linux - Vue 'Obélisque'",
    scene=dict(
        xaxis=dict(visible=False),
        yaxis=dict(visible=False),
        zaxis=dict(title="Adresses Virtuelles (Haut vers Bas)", color="white"),
        bgcolor="black",
    ),
    paper_bgcolor="black",
    font=dict(color="white"),
    margin=dict(r=0, l=0, b=0, t=50),
)

fig.show()
