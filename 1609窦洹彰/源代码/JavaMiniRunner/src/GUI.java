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
            File fileToDelete = new File("./src/Main.vm");
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


        String[] cmd = {"./jackc", "Main.jack"};
        File dir = new File("/Users/white/Desktop/JavaMiniRunner/src");
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
            File f = new File("./src/Main.vm");
            errorMessage.setText("");
            System.out.println("232");
            if (f.exists())
            {
                errorMessage.setText("");
                System.out.println("Done");
                errorMessage.append("Compile Success"+'\n');

                JOptionPane.showMessageDialog(null,"Tokens will be seen in the bottom TextArea!\n");

                File token=new File("./src/Token.txt");

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
                File table=new File("./src/SymbolTable.txt");
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

                File output = new File("./src/error.txt");


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
        File output = new File("./src/Main.vm");

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
        File midCode=new File("./src/MidCode.txt");


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

    public static void main(String[] args)
    {
        new Mycompiler();
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

