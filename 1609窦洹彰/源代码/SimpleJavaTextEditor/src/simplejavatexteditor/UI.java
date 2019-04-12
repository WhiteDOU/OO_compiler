
package simplejavatexteditor;

import javax.swing.*;
import java.awt.*;
import java.awt.datatransfer.DataFlavor;
import java.awt.datatransfer.Transferable;
import java.awt.dnd.DropTarget;
import java.awt.dnd.DropTargetDragEvent;
import java.awt.dnd.DropTargetDropEvent;
import java.awt.dnd.DropTargetEvent;
import java.awt.dnd.DropTargetListener;
import java.awt.event.*;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileReader;
import java.io.FileWriter;
import java.util.ArrayList;
import java.util.Scanner;
import javax.swing.text.DefaultEditorKit;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;

import java.nio.ByteOrder;
import java.util.List;


import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;


import javax.swing.*;

public class UI extends JFrame implements ActionListener {

    private final String[] dragDropExtensionFilter = {".txt", ".dat", ".log", ".xml", ".mf", ".html"};
    private static long serialVersionUID = 1L;
    private final JTextArea textArea;
    private final JMenuBar menuBar;
    private final JComboBox fontSize, fontType;
    private final JMenu menuFile, menuEdit, menuFind, menuAbout;
    private final JMenuItem newFile, openFile, saveFile, close, cut, copy, paste, clearFile, selectAll, quickFind,
            aboutMe, aboutSoftware, wordWrap;
    private final JToolBar mainToolbar;
    JButton newButton, openButton, saveButton, clearButton, quickButton, aboutMeButton, aboutButton, runeButton, boldButton, italicButton;
    private final Action selectAllAction;

    //setup icons - Bold and Italic
    private final ImageIcon boldIcon = new ImageIcon("icons/bold.png");
    private final ImageIcon italicIcon = new ImageIcon("icons/italic.png");

    // setup icons - File Menu
    private final ImageIcon newIcon = new ImageIcon("icons/new.png");
    private final ImageIcon openIcon = new ImageIcon("icons/open.png");
    private final ImageIcon saveIcon = new ImageIcon("icons/save.png");
    private final ImageIcon runIcon = new ImageIcon("icons/run.png");
    private final ImageIcon closeIcon = new ImageIcon("icons/close.png");

    // setup icons - Edit Menu
    private final ImageIcon clearIcon = new ImageIcon("icons/clear.png");
    private final ImageIcon cutIcon = new ImageIcon("icons/cut.png");
    private final ImageIcon copyIcon = new ImageIcon("icons/copy.png");
    private final ImageIcon pasteIcon = new ImageIcon("icons/paste.png");
    private final ImageIcon selectAllIcon = new ImageIcon("icons/selectall.png");
    private final ImageIcon wordwrapIcon = new ImageIcon("icons/wordwrap.png");

    // setup icons - Search Menu
    private final ImageIcon searchIcon = new ImageIcon("icons/search.png");

    // setup icons - Help Menu
    private final ImageIcon aboutMeIcon = new ImageIcon("icons/about_me.png");
    private final ImageIcon aboutIcon = new ImageIcon("icons/about.png");

    private SupportedKeywords kw = new SupportedKeywords();
    private HighlightText languageHighlighter = new HighlightText(Color.GRAY);
    AutoComplete autocomplete;
    private boolean hasListener = false;
    private boolean edit = false;

    public UI() {
        try {
            ImageIcon image = new ImageIcon("icons/ste.png");
            super.setIconImage(image.getImage());
        } catch (Exception ex) {
            ex.printStackTrace();
        }

        // Set the initial size of the window
        setSize(800, 500);

        // Set the title of the window
        setTitle("Untitled | " + SimpleJavaTextEditor.NAME);

        // Set the default close operation (exit when it gets closed)
        setDefaultCloseOperation(EXIT_ON_CLOSE);

        // center the frame on the monitor
        setLocationRelativeTo(null);

        // Set a default font for the TextArea
        textArea = new JTextArea("", 0, 0);
        textArea.setFont(new Font("Century Gothic", Font.PLAIN, 12));
        textArea.setTabSize(2);
        textArea.setFont(new Font("Century Gothic", Font.PLAIN, 12));
        textArea.setTabSize(2);

        /* SETTING BY DEFAULT WORD WRAP ENABLED OR TRUE */
        textArea.setLineWrap(true);
        DropTarget dropTarget = new DropTarget(textArea, dropTargetListener);

        // Set an higlighter to the JTextArea
        textArea.addKeyListener(new KeyAdapter() {
            public void keyPressed(KeyEvent ke) {
                edit = true;
                languageHighlighter.highLight(textArea, kw.getCppKeywords());
                languageHighlighter.highLight(textArea, kw.getJavaKeywords());
            }
        });

        JScrollPane scrollPane = new JScrollPane(textArea);
        textArea.setWrapStyleWord(true);
        scrollPane.setVerticalScrollBarPolicy(JScrollPane.VERTICAL_SCROLLBAR_ALWAYS);
        getContentPane().setLayout(new BorderLayout()); // the BorderLayout bit makes it fill it automatically
        JPanel panel = new JPanel(new BorderLayout());
        panel.add(scrollPane);
        getContentPane().add(panel);

        // Set the Menus
        menuFile = new JMenu("File");
        menuEdit = new JMenu("Edit");
        menuFind = new JMenu("Search");
        menuAbout = new JMenu("About");
        //Font Settings menu

        // Set the Items Menu
        newFile = new JMenuItem("New", newIcon);
        openFile = new JMenuItem("Open", openIcon);
        saveFile = new JMenuItem("Save", saveIcon);
        close = new JMenuItem("Quit", closeIcon);
        clearFile = new JMenuItem("Clear", clearIcon);
        quickFind = new JMenuItem("Quick", searchIcon);
        aboutMe = new JMenuItem("About Me", aboutMeIcon);
        aboutSoftware = new JMenuItem("About Software", aboutIcon);

        menuBar = new JMenuBar();
        menuBar.add(menuFile);
        menuBar.add(menuEdit);
        menuBar.add(menuFind);

        menuBar.add(menuAbout);

        this.setJMenuBar(menuBar);

        // Set Actions:
        selectAllAction = new SelectAllAction("Select All", clearIcon, "Select all text", new Integer(KeyEvent.VK_A),
                textArea);

        this.setJMenuBar(menuBar);

        // New File
        newFile.addActionListener(this);  // Adding an action listener (so we know when it's been clicked).
        newFile.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_N, InputEvent.CTRL_MASK)); // Set a keyboard shortcut
        menuFile.add(newFile); // Adding the file menu

        // Open File
        openFile.addActionListener(this);
        openFile.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_O, InputEvent.CTRL_MASK));
        menuFile.add(openFile);

        // Save File
        saveFile.addActionListener(this);
        saveFile.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_S, InputEvent.CTRL_MASK));
        menuFile.add(saveFile);

        // Close File
        /*
         * Along with our "CTRL+F4" shortcut to close the window, we also have
         * the default closer, as stated at the beginning of this tutorial. this
         * means that we actually have TWO shortcuts to close:
         * 1) the default close operation (example, Alt+F4 on Windows)
         * 2) CTRL+F4, which we are
         * about to define now: (this one will appear in the label).
         */
        close.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_Q, InputEvent.CTRL_MASK));
        close.addActionListener(this);
        menuFile.add(close);

        // Select All Text
        selectAll = new JMenuItem(selectAllAction);
        selectAll.setText("Select All");
        selectAll.setIcon(selectAllIcon);
        selectAll.setToolTipText("Select All");
        selectAll.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_A, InputEvent.CTRL_MASK));
        menuEdit.add(selectAll);

        // Clear File (Code)
        clearFile.addActionListener(this);
        clearFile.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_K, InputEvent.CTRL_MASK));
        menuEdit.add(clearFile);

        // Cut Text
        cut = new JMenuItem(new DefaultEditorKit.CutAction());
        cut.setText("Cut");
        cut.setIcon(cutIcon);
        cut.setToolTipText("Cut");
        cut.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_X, InputEvent.CTRL_MASK));
        menuEdit.add(cut);

        // WordWrap
        wordWrap = new JMenuItem();
        wordWrap.setText("Word Wrap");
        wordWrap.setIcon(wordwrapIcon);
        wordWrap.setToolTipText("Word Wrap");

        //Short cut key or key stroke
        wordWrap.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_W, InputEvent.CTRL_MASK));
        menuEdit.add(wordWrap);

        /* CODE FOR WORD WRAP OPERATION
         * BY DEFAULT WORD WRAPPING IS ENABLED.
         */
        wordWrap.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent ev) {
                // If wrapping is false then after clicking on menuitem the word wrapping will be enabled
                if (textArea.getLineWrap() == false) {
                    /* Setting word wrapping to true */
                    textArea.setLineWrap(true);
                } else {
                    // else  if wrapping is true then after clicking on menuitem the word wrapping will be disabled
                    /* Setting word wrapping to false */
                    textArea.setLineWrap(false);
                }
            }
        });

        // Copy Text
        copy = new JMenuItem(new DefaultEditorKit.CopyAction());
        copy.setText("Copy");
        copy.setIcon(copyIcon);
        copy.setToolTipText("Copy");
        copy.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_C, InputEvent.CTRL_MASK));
        menuEdit.add(copy);

        // Paste Text
        paste = new JMenuItem(new DefaultEditorKit.PasteAction());
        paste.setText("Paste");
        paste.setIcon(pasteIcon);
        paste.setToolTipText("Paste");
        paste.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_V, InputEvent.CTRL_MASK));
        menuEdit.add(paste);

        // Find Word
        quickFind.addActionListener(this);
        quickFind.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_F, InputEvent.CTRL_MASK));
        menuFind.add(quickFind);

        // About Me
        aboutMe.addActionListener(this);
        aboutMe.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_F1, 0));
        menuAbout.add(aboutMe);

        // About Software
        aboutSoftware.addActionListener(this);
        aboutSoftware.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_F2, 0));
        menuAbout.add(aboutSoftware);

        mainToolbar = new JToolBar();
        this.add(mainToolbar, BorderLayout.NORTH);
        // used to create space between button groups
        newButton = new JButton(newIcon);
        newButton.setToolTipText("New");
        newButton.addActionListener(this);
        mainToolbar.add(newButton);
        mainToolbar.addSeparator();

        openButton = new JButton(openIcon);
        openButton.setToolTipText("Open");
        openButton.addActionListener(this);
        mainToolbar.add(openButton);
        mainToolbar.addSeparator();

        saveButton = new JButton(saveIcon);
        saveButton.setToolTipText("Save");
        saveButton.addActionListener(this);
        mainToolbar.add(saveButton);
        mainToolbar.addSeparator();

        clearButton = new JButton(clearIcon);
        clearButton.setToolTipText("Clear All");
        clearButton.addActionListener(this);
        mainToolbar.add(clearButton);
        mainToolbar.addSeparator();

        quickButton = new JButton(searchIcon);
        quickButton.setToolTipText("Quick Search");
        quickButton.addActionListener(this);
        mainToolbar.add(quickButton);
        mainToolbar.addSeparator();

        aboutMeButton = new JButton(aboutMeIcon);
        aboutMeButton.setToolTipText("About Me");
        aboutMeButton.addActionListener(this);
        mainToolbar.add(aboutMeButton);
        mainToolbar.addSeparator();

        aboutButton = new JButton(aboutIcon);
        aboutButton.setToolTipText("About NotePad PH");
        aboutButton.addActionListener(this);
        mainToolbar.add(aboutButton);
        mainToolbar.addSeparator();

        runeButton = new JButton(runIcon);
        runeButton.setToolTipText("Run");
        runeButton.addActionListener(this);
        mainToolbar.add(runeButton);
        mainToolbar.addSeparator();

        boldButton = new JButton(boldIcon);
        boldButton.setToolTipText("Bold");
        boldButton.addActionListener(this);
        mainToolbar.add(boldButton);
        mainToolbar.addSeparator();

        italicButton = new JButton(italicIcon);
        italicButton.setToolTipText("Italic");
        italicButton.addActionListener(this);
        mainToolbar.add(italicButton);
        mainToolbar.addSeparator();
        /**
         * **************** FONT SETTINGS SECTION **********************
         */
        //FONT FAMILY SETTINGS SECTION START
        fontType = new JComboBox();

        //GETTING ALL AVAILABLE FONT FOMILY NAMES
        String[] fonts = GraphicsEnvironment.getLocalGraphicsEnvironment().getAvailableFontFamilyNames();

        for (int i = 0; i < fonts.length; i++) {
            //Adding font family names to font[] array
            fontType.addItem(fonts[i]);
        }
        //Setting maximize size of the fontType ComboBox
        fontType.setMaximumSize(new Dimension(170, 30));
        fontType.setToolTipText("Font Type");
        mainToolbar.add(fontType);
        mainToolbar.addSeparator();

        //Adding Action Listener on fontType JComboBox
        fontType.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent ev) {
                //Getting the selected fontType value from ComboBox
                String p = fontType.getSelectedItem().toString();
                //Getting size of the current font or text
                int s = textArea.getFont().getSize();
                textArea.setFont(new Font(p, Font.PLAIN, s));
            }
        });

        //FONT FAMILY SETTINGS SECTION END
        //FONT SIZE SETTINGS START
        fontSize = new JComboBox();

        for (int i = 5; i <= 100; i++) {
            fontSize.addItem(i);
        }
        fontSize.setMaximumSize(new Dimension(70, 30));
        fontSize.setToolTipText("Font Size");
        mainToolbar.add(fontSize);

        fontSize.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent ev) {
                String sizeValue = fontSize.getSelectedItem().toString();
                int sizeOfFont = Integer.parseInt(sizeValue);
                String fontFamily = textArea.getFont().getFamily();

                Font font1 = new Font(fontFamily, Font.PLAIN, sizeOfFont);
                textArea.setFont(font1);
            }
        });
        //FONT SIZE SETTINGS SECTION END
    }

    @Override
    protected void processWindowEvent(WindowEvent e) {
        if (e.getID() == WindowEvent.WINDOW_CLOSING) {
            if (edit) {
                Object[] options = {"Save and exit", "No Save and exit", "Return"};
                int n = JOptionPane.showOptionDialog(this, "Do you want to save the file ?", "Question",
                        JOptionPane.YES_NO_CANCEL_OPTION, JOptionPane.QUESTION_MESSAGE, null, options, options[1]);
                if (n == 0) {// save and exit
                    saveFile();
                    this.dispose();// dispose all resources and close the application
                } else if (n == 1) {// no save and exit
                    this.dispose();// dispose all resources and close the application
                }
            } else {
                System.exit(99);
            }
        }
    }

    // Make the TextArea available to the autocomplete handler
    protected JTextArea getEditor() {
        return textArea;
    }

    // Enable autocomplete option
    public void enableAutoComplete(File file) {
        if (hasListener) {
            textArea.getDocument().removeDocumentListener(autocomplete);
            hasListener = false;
        }

        ArrayList<String> arrayList;
        String[] list = kw.getSupportedLanguages();

        for (int i = 0; i < list.length; i++) {
            if (file.getName().endsWith(list[i])) {
                switch (i) {
                    case 0:
                        String[] jk = kw.getJavaKeywords();
                        arrayList = kw.setKeywords(jk);
                        autocomplete = new AutoComplete(this, arrayList);
                        textArea.getDocument().addDocumentListener(autocomplete);
                        hasListener = true;
                        break;
                    case 1:
                        String[] ck = kw.getCppKeywords();
                        arrayList = kw.setKeywords(ck);
                        autocomplete = new AutoComplete(this, arrayList);
                        textArea.getDocument().addDocumentListener(autocomplete);
                        hasListener = true;
                        break;
                }
            }
        }
    }

    public void actionPerformed(ActionEvent e) {
        // If the source of the event was our "close" option
        if (e.getSource() == close ) {
            if (edit) {
                Object[] options = {"Save and exit", "No Save and exit", "Return"};
                int n = JOptionPane.showOptionDialog(this, "Do you want to save the file ?", "Question",
                        JOptionPane.YES_NO_CANCEL_OPTION, JOptionPane.QUESTION_MESSAGE, null, options, options[2]);
                if (n == 0) {// save and exit
                    saveFile();
                    this.dispose();// dispose all resources and close the application
                } else if (n == 1) {// no save and exit
                    this.dispose();// dispose all resources and close the application
                }
            } else {
                this.dispose();// dispose all resources and close the application
            }
        } // If the source was the "new" file option
        else if (e.getSource()==runeButton)
        {
            new Mycompiler();
        }
        else if (e.getSource() == newFile || e.getSource() == newButton) {
            if (edit) {
                Object[] options = {"Save", "No Save", "Return"};
                int n = JOptionPane.showOptionDialog(this, "Do you want to save the file at first ?", "Question",
                        JOptionPane.YES_NO_CANCEL_OPTION, JOptionPane.QUESTION_MESSAGE, null, options, options[2]);
                if (n == 0) {// save
                    saveFile();
                    edit = false;
                } else if (n == 1) {
                    edit = false;
                    FEdit.clear(textArea);
                }
            } else {
                FEdit.clear(textArea);
            }

        } // If the source was the "open" option
        else if (e.getSource() == openFile || e.getSource() == openButton) {
            JFileChooser open = new JFileChooser(); // open up a file chooser (a dialog for the user to  browse files to open)
            int option = open.showOpenDialog(this); // get the option that the user selected (approve or cancel)

            /*
             * NOTE: because we are OPENing a file, we call showOpenDialog~ if
             * the user clicked OK, we have "APPROVE_OPTION" so we want to open
             * the file
             */
            if (option == JFileChooser.APPROVE_OPTION) {
                FEdit.clear(textArea); // clear the TextArea before applying the file contents
                try {
                    File openFile = open.getSelectedFile();
                    setTitle(openFile.getName() + " | " + SimpleJavaTextEditor.NAME);
                    Scanner scan = new Scanner(new FileReader(openFile.getPath()));
                    while (scan.hasNext()) {
                        textArea.append(scan.nextLine() + "\n");
                    }

                    enableAutoComplete(openFile);
                } catch (Exception ex) { // catch any exceptions, and...
                    // ...write to the debug console
                    System.err.println(ex.getMessage());
                }
            }
        } // If the source of the event was the "save" option
        else if (e.getSource() == saveFile || e.getSource() == saveButton) {
            saveFile();
        }// If the source of the event was the "Bold" button
        else if (e.getSource() == boldButton) {
            if (textArea.getFont().getStyle() == Font.BOLD) {
                textArea.setFont(textArea.getFont().deriveFont(Font.PLAIN));
            } else {
                textArea.setFont(textArea.getFont().deriveFont(Font.BOLD));
            }
        }// If the source of the event was the "Italic" button
        else if (e.getSource() == italicButton) {
            if (textArea.getFont().getStyle() == Font.ITALIC) {
                textArea.setFont(textArea.getFont().deriveFont(Font.PLAIN));
            } else {
                textArea.setFont(textArea.getFont().deriveFont(Font.ITALIC));
            }
        }
        // Clear File (Code)
        if (e.getSource() == clearFile || e.getSource() == clearButton) {

            Object[] options = {"Yes", "No"};
            int n = JOptionPane.showOptionDialog(this, "Are you sure to clear the text Area ?", "Question",
                    JOptionPane.YES_NO_CANCEL_OPTION, JOptionPane.QUESTION_MESSAGE, null, options, options[1]);
            if (n == 0) {// clear
                FEdit.clear(textArea);
            }
        }
        // Find
        if (e.getSource() == quickFind || e.getSource() == quickButton) {
            new Find(textArea);
        } // About Me
        else if (e.getSource() == aboutMe || e.getSource() == aboutMeButton) {
            new About(this).me();
        } // About Software
        else if (e.getSource() == aboutSoftware || e.getSource() == aboutButton) {
            new About(this).software();
        }
    }

    class SelectAllAction extends AbstractAction {

        /**
         * Used for Select All function
         */
        private static final long serialVersionUID = 1L;

        public SelectAllAction(String text, ImageIcon icon, String desc, Integer mnemonic, final JTextArea textArea) {
            super(text, icon);
            putValue(SHORT_DESCRIPTION, desc);
            putValue(MNEMONIC_KEY, mnemonic);
        }

        public void actionPerformed(ActionEvent e) {
            textArea.selectAll();
        }
    }

    private void saveFile() {
        // Open a file chooser
        JFileChooser fileChoose = new JFileChooser();
        // Open the file, only this time we call
        int option = fileChoose.showSaveDialog(this);

        /*
             * ShowSaveDialog instead of showOpenDialog if the user clicked OK
             * (and not cancel)
         */
        if (option == JFileChooser.APPROVE_OPTION) {
            try {
                File openFile = fileChoose.getSelectedFile();
                setTitle(openFile.getName() + " | " + SimpleJavaTextEditor.NAME);

                BufferedWriter out = new BufferedWriter(new FileWriter(openFile.getPath()));
                out.write(textArea.getText());
                out.close();

                enableAutoComplete(openFile);
                edit = false;
            } catch (Exception ex) { // again, catch any exceptions and...
                // ...write to the debug console
                System.err.println(ex.getMessage());
            }
        }
    }
    DropTargetListener dropTargetListener = new DropTargetListener() {

        @Override
        public void dragEnter(DropTargetDragEvent e) {
        }

        @Override
        public void dragExit(DropTargetEvent e) {
        }

        @Override
        public void dragOver(DropTargetDragEvent e) {
        }

        @Override
        public void drop(DropTargetDropEvent e) {
            if (edit) {
                Object[] options = {"Save", "No Save", "Return"};
                int n = JOptionPane.showOptionDialog(UI.this, "Do you want to save the file at first ?", "Question",
                        JOptionPane.YES_NO_CANCEL_OPTION, JOptionPane.QUESTION_MESSAGE, null, options, options[2]);
                if (n == 0) {// save
                    UI.this.saveFile();
                    edit = false;
                } else if (n == 1) {
                    edit = false;
                    FEdit.clear(textArea);
                } else if (n == 2) {
                    e.rejectDrop();
                    return;
                }
            }
            try {
                Transferable tr = e.getTransferable();
                DataFlavor[] flavors = tr.getTransferDataFlavors();
                for (int i = 0; i < flavors.length; i++) {
                    if (flavors[i].isFlavorJavaFileListType()) {
                        e.acceptDrop(e.getDropAction());

                        try {
                            String fileName = tr.getTransferData(flavors[i]).toString().replace("[", "").replace("]", "");

                            // Allowed file filter extentions for drag and drop
                            boolean extensionAllowed = false;
                            for (int j = 0; j < dragDropExtensionFilter.length; j++) {
                                if (fileName.endsWith(dragDropExtensionFilter[j])) {
                                    extensionAllowed = true;
                                    break;
                                }
                            }
                            if (!extensionAllowed) {
                                JOptionPane.showMessageDialog(UI.this, "This file is not allowed for drag & drop", "Error", JOptionPane.ERROR_MESSAGE);

                            } else {
                                FileInputStream fis = new FileInputStream(new File(fileName));
                                byte[] ba = new byte[fis.available()];
                                fis.read(ba);
                                textArea.setText(new String(ba));
                                fis.close();
                            }
                        } catch (Exception ex) {
                            ex.printStackTrace();
                        }
                        e.dropComplete(true);
                        return;
                    }
                }
            } catch (Throwable t) {
                t.printStackTrace();
            }
            e.rejectDrop();
        }

        @Override
        public void dropActionChanged(DropTargetDragEvent e) {
        }
    };

}

class Mycompiler extends JFrame
{
    private JMenuBar menuBar;
    private JMenu fileMenu;
    private JMenu projectMenu;
    private JMenu helpMenu;
    private JTextArea jTextArea;
    private JScrollPane jScrollPane;
    private JMenuItem openItem, closeItem, saveItem, aboutItem;
    private JMenuItem compileItem, runItem;
    private JTextArea textAreaAfterCompile;
    private JScrollPane textAfterCompileScroll;
    //mid-code
    private JScrollPane SPaneOfTheMidCode;
    private JTextArea textOfMidCode;

    //ClassTable
    private JScrollPane tableScroll;
    private JTextArea tableArea;



    private FileDialog open, save;
    private File file;

    private JTextArea errorMessage;
    private JScrollPane errorPane;
    private JPanel outputPanle;


    // private  List<Token> allToken;
   /* private List<PerSymbol> allSymbol;
    private List<PerPcode> allPcode;*/
    private List<String> errors;
    private String consoleMessage;
    private int readNum = 0;
    private List<String> output;
    private boolean success = false;


    public Mycompiler()
    {
        init();

    }


    private void init()
    {


        JFrame frame = new JFrame("JavaMiniRunner");
        frame.setBounds(300, 300, 700, 450);
        frame.setDefaultCloseOperation(EXIT_ON_CLOSE);

        menuBar = new JMenuBar();//菜单栏
        fileMenu = new JMenu("File");
        projectMenu = new JMenu("Project");
        helpMenu = new JMenu("Help");

        jTextArea = new JTextArea(10, 40);
        jTextArea.setFont(new Font("Monospaced", 1, 20));
        jTextArea.setLineWrap(true);//到达指定宽度则换行
        //应当首先利用构造函数指定JScrollPane的控制对象，此处为JTextArea，然后再添加JScrollPane
        //添加进面板
        jScrollPane = new JScrollPane(jTextArea);
        //设置滚动条自动出现
        jScrollPane.setHorizontalScrollBarPolicy(JScrollPane.HORIZONTAL_SCROLLBAR_AS_NEEDED);
        jScrollPane.setVerticalScrollBarPolicy(JScrollPane.VERTICAL_SCROLLBAR_AS_NEEDED);
        jScrollPane.setViewportView(jTextArea);

        textAreaAfterCompile = new JTextArea(10, 20);
        textAreaAfterCompile.setFont(new Font("Monospaced", 1, 20));
        textAreaAfterCompile.setLineWrap(true);

        textAfterCompileScroll = new JScrollPane(textAreaAfterCompile);
        textAfterCompileScroll.setHorizontalScrollBarPolicy(JScrollPane.HORIZONTAL_SCROLLBAR_AS_NEEDED);
        textAfterCompileScroll.setVerticalScrollBarPolicy(JScrollPane.VERTICAL_SCROLLBAR_AS_NEEDED);
        textAfterCompileScroll.setViewportView(textAreaAfterCompile);

        textOfMidCode=new JTextArea(10,40);
        textOfMidCode.setFont(new Font("Monospaced",1,20));
        textOfMidCode.setLineWrap(true);

        SPaneOfTheMidCode=new JScrollPane(textOfMidCode);
        SPaneOfTheMidCode.setHorizontalScrollBarPolicy(JScrollPane.HORIZONTAL_SCROLLBAR_AS_NEEDED);
        SPaneOfTheMidCode.setVerticalScrollBarPolicy(JScrollPane.VERTICAL_SCROLLBAR_AS_NEEDED);
        SPaneOfTheMidCode.setViewportView(textOfMidCode);


        openItem = new JMenuItem("Open");
        saveItem = new JMenuItem("Save");
        closeItem = new JMenuItem("Close");
        aboutItem = new JMenuItem("About");
        compileItem = new JMenuItem("Compile");
        runItem = new JMenuItem("Run");

        menuBar.add(fileMenu);
        menuBar.add(projectMenu);
        menuBar.add(helpMenu);
        fileMenu.add(openItem);
        fileMenu.add(saveItem);
        fileMenu.add(closeItem);
        projectMenu.add(compileItem);
        projectMenu.add(runItem);
        helpMenu.add(aboutItem);

        errorMessage = new JTextArea();
        errorPane = new JScrollPane(errorMessage);
        errorPane.setPreferredSize(new Dimension(700, 100));

        tableArea=new JTextArea();
        tableScroll=new JScrollPane(tableArea);
        tableScroll.setPreferredSize(new Dimension(700,100));

        JPanel jPanel=new JPanel(new GridLayout(2,3));

        frame.add(menuBar,BorderLayout.NORTH);
        frame.add(jScrollPane,BorderLayout.WEST);
        frame.add(textAfterCompileScroll,BorderLayout.EAST);
        frame.add(SPaneOfTheMidCode,BorderLayout.CENTER);
        frame.add(errorPane, BorderLayout.SOUTH);


        open = new FileDialog(frame, "打开文档", FileDialog.LOAD);
        save = new FileDialog(frame, "保存文档", FileDialog.SAVE);

        File output1 = new File("/Users/white/Desktop/SimpleJavaTextEditor/src/simplejavatexteditor/Main.jack");



        jTextArea.setText("");//打开文件之前清空文本区域

        try
        {
            BufferedReader br = new BufferedReader(new FileReader(output1));
            String line = null;
            while ((line = br.readLine()) != null)
            {
                //将给定文本追加到文档结尾。如果模型为 null 或者字符串为 null 或空，则不执行任何操作。
                //虽然大多数 Swing 方法不是线程安全的，但此方法是线程安全的。
                jTextArea.append(line + "\n");
            }
        } catch (IOException ex)
        {
            throw new RuntimeException("读取失败！");
        }

        Event();
        frame.setVisible(true);
    }

    private void Event()
    {
        closeItem.addActionListener(new ActionListener()
        {
            @Override
            public void actionPerformed(ActionEvent e)
            {
                System.exit(0);
            }
        });

        aboutItem.addActionListener(new ActionListener()
        {
            @Override
            public void actionPerformed(ActionEvent e)
            {
                JOptionPane.showMessageDialog(null, "JavaMiniCompiler\n"
                        + "made by White\ndouhuanzhang@gmail.com");
            }
        });

        openItem.addActionListener(new ActionListener()
        {//菜单条目监听：打开
            public void actionPerformed(ActionEvent e)
            {
                open.setVisible(true);

                String dirPath = open.getDirectory();
                String fileName = open.getFile();
                if (dirPath == null || fileName == null)
                {
                    return;
                }
                file = new File(dirPath, fileName);

                jTextArea.setText("");//打开文件之前清空文本区域

                try
                {
                    BufferedReader br = new BufferedReader(new FileReader(file));
                    String line = null;
                    while ((line = br.readLine()) != null)
                    {
                        //将给定文本追加到文档结尾。如果模型为 null 或者字符串为 null 或空，则不执行任何操作。
                        //虽然大多数 Swing 方法不是线程安全的，但此方法是线程安全的。
                        jTextArea.append(line + "\r\n");
                    }
                } catch (IOException ex)
                {
                    throw new RuntimeException("读取失败！");
                }
            }
        });

        saveItem.addActionListener(new ActionListener()
        {//菜单条目监听：保存
            public void actionPerformed(ActionEvent e)
            {
                if (file == null)
                {
                    newFile();
                }
                saveFile();
            }
        });

        compileItem.addActionListener(new ActionListener()
        {
            public void actionPerformed(ActionEvent e)
            {
                compile();
            }
        });


        runItem.addActionListener(new ActionListener()
        {
            public void actionPerformed(ActionEvent e)
            {
                if (success)
                {
                    //该函数还没有做好
                    //   run();
                }
            }
        });
    }

    private void newFile()
    {
        if (file == null)
        {
            save.setVisible(true);
            String dirPath = save.getDirectory();
            String fileName = save.getFile();
            if (dirPath == null || fileName == null)
            {
                return;
            }
            file = new File(dirPath, fileName);
        }
    }

    private void compile()
    {
        if (file == null)
        {
            JOptionPane.showMessageDialog(null, "Please save the file");
            newFile();
        }
        saveFile();

        try
        {
            File fileToDelete = new File("/Users/white/Desktop/SimpleJavaTextEditor/src/simplejavatexteditor//Main.vm");
            if (fileToDelete.delete())
            {
                System.out.println(fileToDelete.getName() + " 文件已被删除！");
            } else
            {
                System.out.println("文件删除失败！");
            }
        } catch (Exception e)
        {
            e.printStackTrace();
        }


        String[] cmd = {"/Users/white/Desktop/SimpleJavaTextEditor/src/simplejavatexteditor/jackc", "Main.jack"};
        File dir = new File("/Users/white/Desktop/SimpleJavaTextEditor/src/simplejavatexteditor");
        String[] envp = {};
        Process process = null;
        try
        {
            Runtime runtime = Runtime.getRuntime();
            process = runtime.exec(cmd, envp, dir);

            while (process.isAlive())
            {
                Thread.sleep(10);
            }
            System.out.println("Done");
            System.out.println("Done");
            File f = new File("/Users/white/Desktop/SimpleJavaTextEditor/src/simplejavatexteditor/Main.vm");
            errorMessage.setText("");
            System.out.println("232");
            if (f.exists())
            {
                errorMessage.setText("");
                System.out.println("Done");
                errorMessage.append("Compile Success"+'\n');

                JOptionPane.showMessageDialog(null,"Tokens will be seen in the bottom TextArea!\n");

                File token=new File("/Users/white/Desktop/SimpleJavaTextEditor/src/simplejavatexteditor/Token.txt");

                try
                {
                    BufferedReader tokenReader=new BufferedReader(new FileReader(token));
                    String line=null;
                    while ((line=tokenReader.readLine())!=null)
                    {
                        errorMessage.append(line+'\n');
                    }
                }
                catch (IOException e)
                {
                    e.printStackTrace();
                }
                File table=new File("/Users/white/Desktop/SimpleJavaTextEditor/src/simplejavatexteditor/SymbolTable.txt");
                textOfMidCode.setText("");
                textOfMidCode.append("Here're the Symbol Table as follows::"+"\n");
                try
                {
                    BufferedReader tableReader=new BufferedReader(new FileReader(table));
                    String line=null;
                    while ((line=tableReader.readLine())!=null)
                    {
                        textOfMidCode.append(line+'\n');
                    }

                }
                catch (IOException e)
                {
                    e.printStackTrace();
                }

            } else
            {
                errorMessage.append("Failure , Please check your code and input again!\n");
                errorMessage.append("Here are the program Errors:\n");

                File output = new File("/Users/white/Desktop/SimpleJavaTextEditor/src/simplejavatexteditor/error.txt");


                try
                {
                    BufferedReader br = new BufferedReader(new FileReader(output));
                    String line = null;
                    while ((line = br.readLine()) != null)
                    {
                        //将给定文本追加到文档结尾。如果模型为 null 或者字符串为 null 或空，则不执行任何操作。
                        //虽然大多数 Swing 方法不是线程安全的，但此方法是线程安全的。
                        errorMessage.append(line + "\n");
                    }

                } catch (IOException ex)
                {
                    throw new RuntimeException("读取失败！");
                }

            }
        } catch (Exception e)
        {
            e.printStackTrace();
        } finally
        {
            if (process != null)
            {
                process.destroy();
            }
        }
        String dirPath = open.getDirectory();
        String fileName = open.getFile();
        if (dirPath == null || fileName == null)
        {
            return;
        }
        File output = new File("/Users/white/Desktop/SimpleJavaTextEditor/src/simplejavatexteditor/Main.vm");

        JLabel jLabel = new JLabel("目标代码");
        textAreaAfterCompile.add(jLabel);
        textAreaAfterCompile.setText("");//打开文件之前清空文本区域

        try
        {
            BufferedReader br = new BufferedReader(new FileReader(output));
            String line = null;
            while ((line = br.readLine()) != null)
            {
                //将给定文本追加到文档结尾。如果模型为 null 或者字符串为 null 或空，则不执行任何操作。
                //虽然大多数 Swing 方法不是线程安全的，但此方法是线程安全的。
                textAreaAfterCompile.append(line + "\n");
            }
        } catch (IOException ex)
        {
            throw new RuntimeException("读取失败！");
        }

        File midCode=new File("/Users/white/Desktop/SimpleJavaTextEditor/src/simplejavatexteditor/MidCode.txt");


        textOfMidCode.append("Here're the intermediate code::" +"\n");

        try
        {
            BufferedReader br=new BufferedReader(new FileReader(midCode));
            String line=null;
            while ((line=br.readLine())!=null)
            {
                textOfMidCode.append(line+'\n');
            }
        }
        catch (IOException e)
        {
            throw new RuntimeException("读取中间代码失败");
        }
    }



    //保存文件
    private void saveFile()
    {
        try
        {
            BufferedWriter bw = new BufferedWriter(new FileWriter(file));
            String text = jTextArea.getText();
            bw.write(text);
            bw.close();
        } catch (IOException ex)
        {
            throw new RuntimeException();
        }
        if(file.exists())
        {
            JOptionPane.showMessageDialog(null,"Save successfully");
        }
    }



    //BackGround
    class BackgroundPanel extends JPanel
    {
        Image im;

        public BackgroundPanel(Image im)
        {
            this.im = im;
            this.setOpaque(true);                    //设置控件不透明,若是false,那么就是透明
        }

        //Draw the background again,继承自Jpanle,是Swing控件需要继承实现的方法,而不是AWT中的Paint()
        public void paintComponent(Graphics g)       //绘图类,详情可见博主的Java 下 java-Graphics
        {
            super.paintComponents(g);
            g.drawImage(im, 0, 0, this.getWidth(), this.getHeight(), this);  //绘制指定图像中当前可用的图像。图像的左上角位于该图形上下文坐标空间的 (x, y)。图像中的透明像素不影响该处已存在的像素

        }
    }
}


