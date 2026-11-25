
import streamlit as st
import pandas as pd
import plotly.graph_objects as go
from datetime import datetime, timedelta

st.set_page_config(
    page_title="MediSafe | Healthcare IoT",
    page_icon="M",
    layout="wide"
)

# ================= DESIGN =================

st.markdown("""
<style>
@import url('https://fonts.googleapis.com/css2?family=Inter:wght@400;500;600;700;800&display=swap');

.stApp {
    background: #080F1D;
    color: #E8EEF7;
    font-family: 'Inter', sans-serif;
}

header[data-testid="stHeader"] {
    background: transparent;
}

.block-container {
    padding: 2rem 2.5rem;
    max-width: 1500px;
}

section[data-testid="stSidebar"] {
    background: #0D1728;
    border-right: 1px solid #1C2A3F;
}

section[data-testid="stSidebar"] .block-container {
    padding: 2rem 1.2rem;
}

h1, h2, h3, p, label {
    color: #E8EEF7 !important;
}

.brand {
    font-size: 28px;
    font-weight: 800;
    color: #32C6D6;
    letter-spacing: 1px;
}

.brand-sub {
    color: #8293AB;
    font-size: 11px;
    letter-spacing: 2px;
    margin-bottom: 35px;
}

.section-label {
    color: #71839E;
    font-size: 11px;
    letter-spacing: 2px;
    font-weight: 700;
    margin-bottom: 12px;
}

.hero {
    background: linear-gradient(120deg, #142B45, #102038);
    border: 1px solid #263B55;
    border-radius: 18px;
    padding: 25px 30px;
    margin: 15px 0 25px 0;
}

.hero-title {
    font-size: 27px;
    font-weight: 800;
    color: #FFFFFF;
}

.hero-sub {
    color: #A0B2C9;
    font-size: 13px;
    margin-top: 8px;
}

.metric-card {
    background: #111E31;
    border: 1px solid #24364E;
    border-radius: 15px;
    padding: 20px;
    min-height: 125px;
}

.metric-label {
    color: #8FA3BE;
    font-size: 13px;
    margin-bottom: 15px;
}

.metric-value {
    color: #F3F7FC;
    font-size: 29px;
    font-weight: 800;
}

.metric-footer {
    color: #31C4CF;
    font-size: 12px;
    margin-top: 8px;
}

.panel {
    background: #111E31;
    border: 1px solid #24364E;
    border-radius: 16px;
    padding: 20px;
    margin-bottom: 15px;
}

.status-normal {
    color: #35D39A;
    font-weight: 700;
}

.status-warning {
    color: #F6C45B;
    font-weight: 700;
}

.status-critical {
    color: #FF6478;
    font-weight: 700;
}

.stButton > button {
    background: #20AFC0;
    color: #FFFFFF;
    border: none;
    border-radius: 9px;
    font-weight: 700;
    padding: 10px 20px;
}

.stButton > button:hover {
    background: #168D9D;
    color: white;
}

div[data-testid="stRadio"] label {
    color: #B8C7DB !important;
}

hr {
    border-color: #25364D;
}

#MainMenu {
    visibility: hidden;
}

footer {
    visibility: hidden;
}
</style>
""", unsafe_allow_html=True)


# ================= SIDEBAR =================

with st.sidebar:

    st.markdown(
        '<div class="brand">MEDISAFE</div>'
        '<div class="brand-sub">HEALTHCARE MONITORING</div>',
        unsafe_allow_html=True
    )

    st.markdown('<div class="section-label">MAIN MENU</div>',
                unsafe_allow_html=True)

    page = st.radio(
        "Navigation",
        [
            "Dashboard",
            "Live Monitoring",
            "Alerts",
            "History",
            "Settings"
        ],
        label_visibility="collapsed"
    )

    st.divider()

    st.markdown('<div class="section-label">SYSTEM STATUS</div>',
                unsafe_allow_html=True)

    st.markdown(
        '<p class="status-normal">● System Demo Mode</p>',
        unsafe_allow_html=True
    )

    st.caption("MediSafe v2.0")


# ================= DEMO DATA =================

now = datetime.now()

dates = [
    now - timedelta(minutes=5 * i)
    for i in range(23, -1, -1)
]

temperatures = [
    4.5, 4.6, 4.7, 4.8, 4.7, 4.9,
    5.0, 4.8, 5.1, 5.0, 4.9, 5.2,
    5.1, 5.0, 5.2, 5.3, 5.1, 5.2,
    5.0, 5.1, 5.2, 5.1, 5.2, 5.2
]

humidities = [
    43, 44, 44, 45, 44, 46,
    45, 44, 43, 45, 46, 45,
    44, 45, 46, 47, 46, 45,
    44, 45, 46, 45, 45, 45
]

df = pd.DataFrame({
    "Date": dates,
    "Temperature": temperatures,
    "Humidity": humidities
})


# ================= HELPERS =================

def metric_card(label, value, footer):
    st.markdown(f"""
    <div class="metric-card">
        <div class="metric-label">{label}</div>
        <div class="metric-value">{value}</div>
        <div class="metric-footer">{footer}</div>
    </div>
    """, unsafe_allow_html=True)


def chart(data, column, title, color, unit):
    fig = go.Figure()

    fig.add_trace(go.Scatter(
        x=data["Date"],
        y=data[column],
        mode="lines",
        line=dict(color=color, width=3),
        fill="tozeroy",
        fillcolor="rgba(32,175,192,0.08)"
    ))

    fig.update_layout(
        title=dict(text=title, font=dict(size=16)),
        paper_bgcolor="rgba(0,0,0,0)",
        plot_bgcolor="rgba(0,0,0,0)",
        font=dict(color="#AFC0D5"),
        margin=dict(l=10, r=10, t=50, b=10),
        height=310,
        xaxis=dict(showgrid=False),
        yaxis=dict(
            title=unit,
            gridcolor="#26364A"
        )
    )

    return fig


# ================= HEADER =================

st.markdown("""
<div class="hero">
    <div class="hero-title">Healthcare Monitoring Center</div>
    <div class="hero-sub">
        Intelligent Medicine Storage Monitoring System
        | Real-time data visualization
    </div>
</div>
""", unsafe_allow_html=True)

st.caption(
    "DEMONSTRATION MODE — Données simulées, connexion ESP32 à venir."
)


# ================= DASHBOARD =================

if page == "Dashboard":

    st.markdown("### Overview")
    st.caption("Vue générale des conditions de stockage")

    c1, c2, c3 = st.columns(3)

    with c1:
        metric_card("TEMPERATURE", "5.2 °C", "Stable • Démonstration")

    with c2:
        metric_card("HUMIDITY", "45 %", "Stable • Démonstration")

    with c3:
        metric_card("STORAGE STATUS", "NORMAL", "Conditions simulées")

    st.write("")

    left, right = st.columns(2)

    with left:
        st.markdown('<div class="panel">', unsafe_allow_html=True)
        st.plotly_chart(
            chart(df, "Temperature",
                  "Temperature Evolution", "#28C5D8", "°C"),
            use_container_width=True
        )
        st.markdown('</div>', unsafe_allow_html=True)

    with right:
        st.markdown('<div class="panel">', unsafe_allow_html=True)
        st.plotly_chart(
            chart(df, "Humidity",
                  "Humidity Evolution", "#67A7FF", "%"),
            use_container_width=True
        )
        st.markdown('</div>', unsafe_allow_html=True)

    st.markdown("### System Information")

    col1, col2 = st.columns(2)

    with col1:
        st.markdown("""
        <div class="panel">
            <div class="metric-label">DEVICE</div>
            <h3>ESP32 Controller</h3>
            <p style="color:#35D39A">● Simulation mode</p>
            <p style="color:#8FA3BE">Sensor: DHT11</p>
        </div>
        """, unsafe_allow_html=True)

    with col2:
        st.markdown("""
        <div class="panel">
            <div class="metric-label">LAST UPDATE</div>
            <h3>Current Session</h3>
            <p style="color:#8FA3BE">Données de démonstration</p>
        </div>
        """, unsafe_allow_html=True)


# ================= LIVE MONITORING =================

elif page == "Live Monitoring":

    st.markdown("### Live Monitoring")
    st.caption("Surveillance des mesures")

    c1, c2 = st.columns(2)

    with c1:
        metric_card("TEMPERATURE", "5.2 °C", "Simulation")

    with c2:
        metric_card("HUMIDITY", "45 %", "Simulation")

    st.markdown("### Sensor Measurements")

    st.dataframe(
        df.sort_values("Date", ascending=False),
        use_container_width=True,
        hide_index=True
    )


# ================= ALERTS =================

elif page == "Alerts":

    st.markdown("### Alerts Center")
    st.caption("Historique des événements du système")

    st.markdown("""
    <div class="panel">
        <p class="status-normal">● SYSTEM NORMAL</p>
        <h3>Aucune alerte détectée</h3>
        <p style="color:#8FA3BE">
        Aucun événement enregistré dans cette démonstration.
        </p>
    </div>
    """, unsafe_allow_html=True)


# ================= HISTORY =================

elif page == "History":

    st.markdown("### Measurement History")

    st.plotly_chart(
        chart(df, "Temperature",
              "Historical Temperature", "#28C5D8", "°C"),
        use_container_width=True
    )

    st.plotly_chart(
        chart(df, "Humidity",
              "Historical Humidity", "#67A7FF", "%"),
        use_container_width=True
    )

    csv = df.to_csv(index=False).encode("utf-8")

    st.download_button(
        "Download CSV",
        csv,
        "medisafe_history.csv",
        "text/csv"
    )


# ================= SETTINGS =================

elif page == "Settings":

    st.markdown("### System Settings")
    st.caption("Configuration des seuils de surveillance")

    st.markdown('<div class="panel">', unsafe_allow_html=True)

    st.markdown("#### Temperature Limits")

    min_temp = st.number_input(
        "Minimum temperature (°C)",
        value=2.0
    )

    max_temp = st.number_input(
        "Maximum temperature (°C)",
        value=8.0
    )

    st.markdown("#### Humidity Limits")

    max_humidity = st.slider(
        "Maximum humidity (%)",
        min_value=10,
        max_value=100,
        value=60
    )

    if st.button("Save Settings"):
        if min_temp >= max_temp:
            st.error("La température minimale doit être inférieure à la maximale.")
        else:
            st.success("Paramètres enregistrés pour cette session.")

    st.markdown('</div>', unsafe_allow_html=True)


# ================= FOOTER =================

st.divider()

st.markdown(
    '<p style="text-align:center;color:#71839E;font-size:12px">'
    'MEDISAFE • SMART HEALTHCARE IOT PROJECT</p>',
    unsafe_allow_html=True
)
