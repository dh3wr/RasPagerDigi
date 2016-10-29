package rasPager;

import java.awt.AWTException;
import java.awt.Canvas;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.Font;
import java.awt.Graphics;
import java.awt.Image;
import java.awt.List;
import java.awt.MenuItem;
import java.awt.PopupMenu;
import java.awt.Rectangle;
import java.awt.SystemTray;
import java.awt.Toolkit;
import java.awt.TrayIcon;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import java.awt.event.WindowEvent;
import java.awt.event.WindowListener;
import java.io.File;
import java.io.FileNotFoundException;
import java.util.Arrays;

import javax.swing.JButton;
import javax.swing.JFileChooser;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTextField;

public class MainWindow extends JFrame {
	private static final long serialVersionUID = 1L;

	private JPanel main;
	private final int WIDTH = 450;
	private final int HEIGHT = 400;

	private TrayIcon trayIcon;

	private List masterList;
	private JLabel statusDisplay;
	private JTextField searchStepWidth;
	private JButton startButton;
	private JTextField masterIP;
	private JTextField port;
	private JTextField freqErrCorr;
	private JTextField modDev;
	private Canvas slotDisplay;

	private Log log = null;

	// write message into log file (log level normal)
	private void log(String message, int type) {
		log(message, type, Log.NORMAL);
	}

	// write message with given log level into log file
	private void log(String message, int type, int logLevel) {
		if (this.log != null) {
			this.log.println(message, type, logLevel);
		}
	}

	// constructor
	public MainWindow(Log log) {
		// set log
		this.log = log;

		// set window preferences
		setTitle("RasPager");
		setResizable(false);
		setDefaultCloseOperation(JFrame.DO_NOTHING_ON_CLOSE);

		// window listener
		addWindowListener(new WindowListener() {

			@Override
			public void windowActivated(WindowEvent arg0) {
			}

			@Override
			public void windowClosed(WindowEvent arg0) {
				System.exit(0);
			}

			@Override
			public void windowClosing(WindowEvent event) {
				// if server is running, ask to quit
				if (Main.running
						&& !showConfirm("Beenden", "Der Server läuft zur Zeit. Wollen Sie wirklich beenden?")) {
					return;
				}

				// if server is running
				if (Main.running) {
					// stop server
					Main.stopServer(false);
				}

				// close log
				Main.closeLog();

				// dispose window
				dispose();
			}

			@Override
			public void windowDeactivated(WindowEvent arg0) {
			}

			@Override
			public void windowDeiconified(WindowEvent arg0) {
				setVisible(true);
			}

			@Override
			public void windowIconified(WindowEvent arg0) {
				setVisible(false);
			}

			@Override
			public void windowOpened(WindowEvent arg0) {
			}

		});

		// main panel
		main = new JPanel(null);
		main.setPreferredSize(new Dimension(WIDTH, HEIGHT));
		main.setBounds(0, 0, WIDTH, HEIGHT);
		add(main);

		// slot display bounds
		Rectangle slotDisplayBounds = new Rectangle(10, 72, 30, 260);

		// slot display label
		JLabel slotDisplayLabel = new JLabel("Slots");
		slotDisplayLabel.setBounds(slotDisplayBounds.x - 2, slotDisplayBounds.y - 22, 50, 18);
		main.add(slotDisplayLabel);

		// slot display
		slotDisplay = new Canvas() {

			private static final long serialVersionUID = 1L;

			@Override
			public void paint(Graphics g) {
				super.paint(g);
				int width = getWidth() - 1;
				int height = getHeight() - 1;
				int x = 15;

				// draw border
				g.drawRect(x, 0, width - x, height);

				int step = getHeight() / 16;

				for (int i = 0, y = step; i < 16; y += step, i++) {

					Font font = g.getFont();
					Color color = g.getColor();

					// if this is allowed slot
					if (Main.timeSlots.getSlotsArray()[i]) {
						// change font and color
						g.setFont(new Font(font.getFontName(), Font.BOLD, font.getSize()));
						g.setColor(Color.green);
					}

					g.drawString("" + Integer.toHexString(i).toUpperCase(), 0, y);
					g.setFont(font);
					g.setColor(color);

					// draw line
					if (i < 16 - 1) {
						g.drawLine(x, y, width, y);
					}

				}

				// if scheduler does not exist, function ends here
				if (Main.scheduler == null) {
					return;
				}

				Color color = g.getColor();
				g.setColor(Color.green);

				// get slot count
				int slot = Main.timeSlots.getCurrentSlotInt(Main.scheduler.getTime());
				int slotCount = Main.timeSlots.checkSlot(String.format("%1x", slot).charAt(0));

				// draw current slots (from slot to slot + slotCount) with
				// different color
				for (int i = 0; i < slotCount; i++) {
					g.fillRect(x + 1, (slot + i) * step + 1, width - x - 1, step - 1);
				}

				g.setColor(Color.yellow);

				g.fillRect(x + 1, slot * step + 1, width - x - 1, step - 1);

				g.setColor(color);

			}

		};
		slotDisplay.setBounds(slotDisplayBounds);
		main.add(slotDisplay);

		// status display bounds
		Rectangle statusDisplayBounds = new Rectangle(120, 10, 120, 18);

		// status display label
		JLabel statusDisplayLabel = new JLabel("Status:");
		statusDisplayLabel.setBounds(statusDisplayBounds.x - 63, statusDisplayBounds.y, 60, 18);
		main.add(statusDisplayLabel);

		// status display
		statusDisplay = new JLabel("getrennt");
		statusDisplay.setBounds(statusDisplayBounds);
		main.add(statusDisplay);

		// server start button bounds
		Rectangle startButtonBounds = new Rectangle(274, 340, 150, 18);

		// server start button
		startButton = new JButton("Server starten");
		startButton.addActionListener(new ActionListener() {

			@Override
			public void actionPerformed(ActionEvent e) {

				if (Main.running) {
					// stop server

					Main.stopServer(false);
					startButton.setText("Server starten");

				} else {
					// start server

					Main.startServer(false);
					startButton.setText("Server stoppen");

				}

			}

		});
		startButton.setBounds(startButtonBounds);
		main.add(startButton);

		// configuration panel bounds
		Rectangle configurationPanelBounds = new Rectangle(statusDisplayBounds.x - 63, 40, 400, 400);

		// configuration panel
		JPanel configurationPanel = new JPanel(null);
		configurationPanel.setBounds(configurationPanelBounds);
		main.add(configurationPanel);

		// master list bounds
		Rectangle masterListBounds = new Rectangle(0, 30, 150, 200);

		// master list label
		JLabel masterListLabel = new JLabel("Master");
		masterListLabel.setBounds(masterListBounds.x, masterListBounds.y - 20, 70, 18);
		configurationPanel.add(masterListLabel);

		// master list
		masterList = new List();
		masterList.setName("masterList");

		// master list pane
		JScrollPane masterListPane = new JScrollPane(masterList);
		masterListPane.setBounds(masterListBounds);
		configurationPanel.add(masterListPane);

		// master add label
		JLabel masterAddLabel = new JLabel("Neuer Master:");
		masterAddLabel.setBounds(masterListBounds.x + masterListBounds.width + 15, masterListBounds.y - 20, 120, 18);
		configurationPanel.add(masterAddLabel);

		// master name field
		masterIP = new JTextField();
		masterIP.setBounds(masterListBounds.x + masterListBounds.width + 12,
				masterListBounds.y + masterAddLabel.getHeight() - 16, 126, 18);
		configurationPanel.add(masterIP);

		// master add button
		JButton masterAdd = new JButton("Hinzufügen");
		masterAdd.addActionListener(new ActionListener() {

			@Override
			public void actionPerformed(ActionEvent arg0) {

				String master = masterIP.getText();
				String[] masters = masterList.getItems();

				// is textfield empty?
				if (master.equals("")) {
					return;
				}

				// check if master is already in list
				for (int i = 0; i < masters.length; i++) {
					if (masters[i].equals(master)) {

						showError("Master hinzufügen", "Master ist bereits in der Liste vorhanden!");
						return;

					}
				}

				// add master
				masterList.add(master);
				// clear textfield
				masterIP.setText("");

			}

		});
		masterAdd.setBounds(masterListBounds.x + masterListBounds.width + 15,
				masterIP.getY() + masterIP.getHeight() + 5, 120, 18);
		configurationPanel.add(masterAdd);

		// master remove button
		JButton masterRemove = new JButton("Löschen");
		masterRemove.addActionListener(new ActionListener() {

			@Override
			public void actionPerformed(ActionEvent e) {
				// check if there is a selection
				if (masterList.getSelectedItem() != null) {
					// ask to remove
					if (showConfirm("Master löschen", "Soll der ausgewählte Master wirklich gelöscht werden?")) {
						// remove master
						masterList.remove(masterList.getSelectedIndex());
					}
				}

			}

		});
		masterRemove.setBounds(masterListBounds.x + masterListBounds.width + 15,
				masterAdd.getY() + masterAdd.getHeight() + 5, 120, 18);
		configurationPanel.add(masterRemove);

		// port bounds
		Rectangle portBounds = new Rectangle(masterListBounds.x + masterListBounds.width - 50,
				masterListBounds.y + masterListBounds.height + 13, 50, 18);

		// port label
		JLabel portLabel = new JLabel("Port:");
		portLabel.setBounds(portBounds.x - 35, portBounds.y, 50, 18);
		configurationPanel.add(portLabel);

		// port
		port = new JTextField();
		port.setBounds(portBounds);
		port.addKeyListener(new KeyListener() {

			@Override
			public void keyTyped(KeyEvent event) {
				char key = event.getKeyChar();

				// check if key is between 0 and 9
				if (key > '9' || key < '0') {
					event.consume();
				}

			}

			@Override
			public void keyReleased(KeyEvent arg0) {
			}

			@Override
			public void keyPressed(KeyEvent arg0) {
			}
		});
		configurationPanel.add(port);

		// freq err corr bounds
		Rectangle freqErrCorrBounds = new Rectangle(masterListBounds.x + masterListBounds.width - 50,
				portBounds.y + portBounds.height + 10, 50, 18);

		JLabel freqErrCorrLabel = new JLabel("Freq Err Corr:");
		freqErrCorrLabel.setBounds(freqErrCorrBounds.x - 90, freqErrCorrBounds.y, 90, 18);
		configurationPanel.add(freqErrCorrLabel);

		freqErrCorr = new JTextField();
		freqErrCorr.setBounds(freqErrCorrBounds);
		freqErrCorr.addKeyListener(new KeyListener() {

			@Override
			public void keyTyped(KeyEvent event) {
				char key = event.getKeyChar();

				String valid = "-0123456789";
				if (valid.indexOf(key) == -1) {
					event.consume();
				}

			}

			@Override
			public void keyReleased(KeyEvent arg0) {
			}

			@Override
			public void keyPressed(KeyEvent arg0) {
			}
		});
		configurationPanel.add(freqErrCorr);
		/////////

		// mode dev bounds
		Rectangle modDevBounds = new Rectangle(masterListBounds.x + masterListBounds.width - 50,
				freqErrCorrBounds.y + freqErrCorrBounds.height + 10, 50, 18);

		JLabel modDevLabel = new JLabel("Mod Dev:");
		modDevLabel.setBounds(modDevBounds.x - 65, modDevBounds.y, 90, 18);
		configurationPanel.add(modDevLabel);

		modDev = new JTextField();
		modDev.setBounds(modDevBounds);
		modDev.addKeyListener(new KeyListener() {

			@Override
			public void keyTyped(KeyEvent event) {
				char key = event.getKeyChar();

				// check if key is between 0 and 9
				if (key > '9' || key < '0') {
					event.consume();
				}

			}

			@Override
			public void keyReleased(KeyEvent arg0) {
			}

			@Override
			public void keyPressed(KeyEvent arg0) {
			}
		});
		configurationPanel.add(modDev);
		/////////

		// config button bounds
		Rectangle configButtonBounds = new Rectangle(47, portBounds.y + portBounds.height + 70, 100, 18);

		// config apply button
		JButton applyButton = new JButton("übernehmen");
		applyButton.addActionListener(new ActionListener() {

			@Override
			public void actionPerformed(ActionEvent e) {
				// set the config
				setConfig();
			}

		});
		applyButton.setBounds(configButtonBounds);
		configurationPanel.add(applyButton);

		configButtonBounds.x += configButtonBounds.width + 10;
		configButtonBounds.width = 100;

		// config load button
		JButton loadButton = new JButton("Laden");
		loadButton.addActionListener(new ActionListener() {

			@Override
			public void actionPerformed(ActionEvent event) {

				// show open dialog
				JFileChooser fileChooser = new JFileChooser("");
				if (fileChooser.showOpenDialog(Main.mainWindow) == JFileChooser.APPROVE_OPTION) {

					// get file name
					File file = fileChooser.getSelectedFile();

					try {
						// try to load config
						Main.config.load(file.getPath());

					} catch (InvalidConfigFileException e) {
						// catch errors
						showError("Config laden", "Die Datei ist keine gültige Config-Datei!");
						log("Load Config # Keine gültige config-Datei", Log.ERROR);

						return;
					}

					// load config (means showing the config)
					loadConfig();

				}

			}

		});
		loadButton.setBounds(configButtonBounds);
		configurationPanel.add(loadButton);

		configButtonBounds.x += configButtonBounds.width + 10;
		configButtonBounds.width = 100;

		// config save button
		JButton saveButton = new JButton("Speichern");
		saveButton.addActionListener(new ActionListener() {

			@Override
			public void actionPerformed(ActionEvent event) {
				// show save dialog
				JFileChooser fileChooser = new JFileChooser("");
				if (fileChooser.showSaveDialog(Main.mainWindow) == JFileChooser.APPROVE_OPTION) {

					// get file name
					File file = fileChooser.getSelectedFile();

					try {
						// try to save config
						setConfig();

						Main.config.save(file.getPath());

					} catch (FileNotFoundException e) {
						// catch errors
						showError("Config speichern", "Die Datei konnte nicht gespeichert werden!");

						log("Save Config # Konnte config-Datei nicht speichern", Log.ERROR);

						return;
					}

				}

			}
		});
		saveButton.setBounds(configButtonBounds);
		configurationPanel.add(saveButton);

		// show window
		pack();
		setVisible(true);

		loadConfig();

		// create tray icon
		Image trayImage = Toolkit.getDefaultToolkit().getImage("icon.ico");

		PopupMenu trayMenu = new PopupMenu("FunkrufSlave");
		MenuItem menuItem = new MenuItem("Anzeigen");
		menuItem.addActionListener(new ActionListener() {

			@Override
			public void actionPerformed(ActionEvent e) {
				// show window
				setExtendedState(JFrame.NORMAL);
				setVisible(true);

			}

		});
		trayMenu.add(menuItem);

		trayIcon = new TrayIcon(trayImage, "FunkrufSlave", trayMenu);
		try {
			SystemTray.getSystemTray().add(trayIcon);
		} catch (AWTException e) {
			e.printStackTrace();
		}

	}

	// set connection status
	public void setStatus(boolean status) {
		this.statusDisplay.setText(status ? "verbunden" : "getrennt");
	}

	public void setConfig() {
		// set port
		Main.config.setPort(Integer.parseInt(port.getText()));
		Main.config.setFreqErrCorr(Integer.parseInt(freqErrCorr.getText()));
		Main.config.setModDev(Integer.parseInt(modDev.getText()));

		// set master
		String master = "";
		for (int i = 0; i < masterList.getItemCount(); i++) {
			master += (i > 0 ? " " : "") + masterList.getItem(i);
		}
		Main.config.setMaster(master);

		if (Main.running) {
			if (showConfirm("Config übernehmen",
					"Der Server läuft bereits. Um die Einstellungen zu übernehmen, muss der Server neugestartet werden. Soll er jetzt neugestartet werden?")) {
				Main.stopServer(false);
				Main.startServer(false);

				startButton.setText("Server stoppen");
			}
		}
	}

	public void loadConfig() {
		// load port
		port.setText("" + Main.config.getPort());
		freqErrCorr.setText("" + Main.config.getFreqErrCorr());
		modDev.setText("" + Main.config.getModDev());

		// load master
		masterList.removeAll();

		String[] master = Main.config.getMaster();
		if (master != null) {
			for (int i = 0; i < master.length; i++) {
				masterList.add(master[i]);
			}
		}
	}

	// reset buttons
	public void resetButtons() {
		startButton.setText("Server starten");
		setStatus(false);
	}

	public void showError(String title, String message) {
		JOptionPane.showMessageDialog(null, message, title, JOptionPane.ERROR_MESSAGE);
	}

	public boolean showConfirm(String title, String message) {
		return JOptionPane.showConfirmDialog(this, message, title, JOptionPane.YES_NO_OPTION) == JOptionPane.YES_OPTION;
	}

	public void drawSlots() {
		slotDisplay.repaint();
	}

	public String getStepWidth() {
		if (searchStepWidth.getText().equals("")) {
			searchStepWidth.setText("" + Main.searchStepWidth);
		}

		return searchStepWidth.getText();
	}
}