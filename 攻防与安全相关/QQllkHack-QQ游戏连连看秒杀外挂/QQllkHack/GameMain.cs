using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;
using System.Windows.Forms;

namespace QQllkHack
{
    struct Point //坐标点
    {
        public int x;//横坐标
        public int y;//纵坐标
    }

    class GameMain
    {
        const string gameCaptian = "QQ游戏 - 连连看角色版";
        //static int gameBase = (int)0x00129FB8;//棋盘基地址
        static byte[,] chess=new byte[11,19];
        static IntPtr GameHwnd;

        [DllImport("User32.dll", EntryPoint = "FindWindow")]
        private static extern IntPtr FindWindow(
        string lpClassName, 
        string lpWindowName
        );

        [DllImport("User32.dll", CharSet = CharSet.Auto)]
        private static extern int GetWindowThreadProcessId(
        IntPtr hwnd, 
        out int ID
        );

        [DllImport("kernel32.dll")]
        private static extern int OpenProcess(
        int dwDesiredAccess, 
        bool bInheritHandle, 
        int dwProcessId
        );

        const int PROCESS_ALL_ACCESS = 0x1F0FFF;
        const int PROCESS_VM_READ = 0x0010;
        const int PROCESS_VM_WRITE = 0x0020;

        [DllImport("kernel32.dll ")]
        private static extern bool ReadProcessMemory(
        int hProcess, 
        int lpBaseAddress, 
        byte[,] lpBuffer, 
        int nSize, 
        out int lpNumberOfBytesRead
        );

        [DllImport("kernel32", EntryPoint = "CloseHandle")]
        private static extern int CloseHandle(
        int hObject
        );

        [DllImport("User32.dll", EntryPoint = "SendMessage")]
        private static extern int SendMessage(
        IntPtr hWnd,   // handle to destination window 
        int Msg,    // message 
        int wParam, // first message parameter 
        int lParam // second message parameter 
        ); 

        const int WM_LBUTTONDOWN = 0x0201;
        const int WM_LBUTTONUP = 0x0202;

        private static bool updateChess(int gameBase)  //更新棋盘
        {
            GameHwnd = FindWindow(null, gameCaptian);
            if (GameHwnd == null)
            {
                MessageBox.Show("游戏未开启");
                return false;
            }
            else
            {
                int processId,//连连看游戏进程号
                gameH,//打开的进程号
                hasRead;//实际读取的字节数
                bool result;
                try
                {
                    GetWindowThreadProcessId(GameHwnd, out processId);
                    //打开游戏进程
                    gameH = OpenProcess(PROCESS_ALL_ACCESS, false, processId);
                    if (ReadProcessMemory(gameH, gameBase, chess, chess.Length, out hasRead) && hasRead > 0)
                    {
                        result = true;
                    }
                    else
                    {
                        MessageBox.Show("未能读入棋盘数据");
                        result = false;
                    }
                    CloseHandle(gameH);
                    return result;
                }
                catch(Exception ex)
                {
                    MessageBox.Show(ex.Message);
                    return false; 
                }
            }
        }

        private static bool checkLine(Point p1,Point p2) //检查直线横竖两点是否连通
        {
            int x, y, temp;
            if (p1.x == p2.x)//横坐标相同，竖线情况
            {
                if (p1.y > p2.y)
                {
                    temp = p1.y; p1.y = p2.y; p2.y = temp;
                }
                for (y = p1.y + 1; y < p2.y; y++)
                {
                    if (chess[y,p1.x] > 0) return false;
                }
            }
            else if (p1.y == p2.y)//纵坐标相同，横线情况
            {
                if (p1.x > p2.x)
                {
                    temp = p1.x; p1.x = p2.x; p2.x = temp;
                }
                for (x = p1.x + 1; x < p2.x; x++)
                {
                    if (chess[p1.y,x] > 0) return false;
                }
            }
            return true;          
        }

        private static bool check2p(Point p1, Point p2) //检测 p1,p2 2个棋子 是否可以消除
        {
            Point pa = p1, pb = p2; //转折点
            int x, y;
            // 如果2点为同一点 则返回假
            if ((p1.x == p2.x) && (p1.y == p2.y)) return false;
            else 
            {
                //如果有一点没有图像 返回假
                if ((chess[p1.y,p1.x] == 0) || (chess[p2.y,p2.x] == 0))
                    return false;
                else
                //如果两点图像不一样 返回假
                    if (chess[p1.y,p1.x] != chess[p2.y,p2.x])
                        return false;
            }
            //纵坐标相同情况
            if (p1.y == p2.y)
            {
                for (y = 0; y < 11; y++)
                {
                    pa.y = pb.y = y;
                    if (chess[pa.y,pa.x] == 0 && chess[pb.y,pb.x] == 0)
                    {
                        if (checkLine(pa, pb) && checkLine(pa, p1) && checkLine(pb, p2)) return true;
                    }
                    else if (pa.x == p1.x && pa.y == p1.y && pb.x == p2.x && pb.y == p2.y)
                    {
                        if (checkLine(p1, p2)) return true;
                    }
                }
                return false;
            }
            pa = p1; pb = p2;
            //横坐标相同情况
            if (p1.x == p2.x)
            {
                for (x = 0; x < 19; x++)
                {
                    pa.x = pb.x = x;
                    if (chess[pa.y,pa.x] == 0 && chess[pb.y,pb.x] == 0)
                    {
                        if (checkLine(pa, pb) && checkLine(pa, p1) && checkLine(pb, p2)) return true;
                    }
                    else if (pa.x == p1.x && pa.y == p1.y && pb.x == p2.x && pb.y == p2.y)
                    {
                        if (checkLine(p1, p2)) return true;
                    }
                }
                return false;
            }
            //x,y 坐标不相同
            pa = p1; pb = p2;
            for (y = 0; y < 11; y++)
            {
                pa.y = pb.y = y;
                if (chess[pa.y,pa.x] == 0 && chess[pb.y,pb.x] == 0)
                {
                    if (checkLine(pa, pb) && checkLine(pa, p1) && checkLine(pb, p2)) return true;
                }
                else if (chess[pa.y,pa.x] == 0 && pb.x == p2.x && pb.y == p2.y)
                {
                    if (checkLine(pa, p1) && checkLine(pa, pb)) return true;
                }
                else if (chess[pb.y,pb.x] == 0 && pa.x == p1.x && pa.y == p1.y)
                {
                    if (checkLine(pb, p2) && checkLine(pa, pb)) return true;
                }
            }
            pa = p1; pb = p2;
            for (x = 0; x < 19; x++)
            {
                pa.x = pb.x = x;
                if (chess[pa.y,pa.x] == 0 && chess[pb.y,pb.x] == 0)
                {
                    if (checkLine(pa, pb) && checkLine(pa, p1) && checkLine(pb, p2)) return true;
                }
                else if (chess[pa.y,pa.x] == 0 && pb.x == p2.x && pb.y == p2.y)
                {
                    if (checkLine(pa, p1) && checkLine(pa, pb)) return true;
                }
                else if (chess[pb.y,pb.x] == 0 && pa.x == p1.x && pa.y == p1.y)
                {
                    if (checkLine(pb, p2) && checkLine(pa, pb)) return true;
                }
            }
            return false;
        }

        private static void click2p(Point p1,Point p2) //点击消除一对棋子
        {
            try
            {
                int lparam;
                //点击p1
                lparam = ((p1.y * 35 + 197) << 16) + (p1.x * 31 + 24);
                SendMessage(GameHwnd, WM_LBUTTONDOWN, 0, lparam);
                SendMessage(GameHwnd, WM_LBUTTONUP, 0, lparam);
                //点击p2
                lparam = ((p2.y * 35 + 197) << 16) + (p2.x * 31 + 24);
                SendMessage(GameHwnd, WM_LBUTTONDOWN, 0, lparam);
                SendMessage(GameHwnd, WM_LBUTTONUP, 0, lparam);       
            }
            catch(Exception ex) 
            {
                MessageBox.Show(ex.Message);
            }
        }

        public static void clickOnePair(int gameBase)//消除一对棋子
        {
            //读出棋盘数据至ChessData 11,19
            if (!updateChess(gameBase)) return;
            //遍历整个棋盘 找出相同类型的一对棋子
            Point p1, p2;
            int i1, i2, j1, j2;
            for (i1 = 0; i1 < 11; i1++)
            {
                for (j1 = 0; j1 < 19; j1++)
                {
                    for (i2 = 0; i2 < 11; i2++)
                        for (j2 = 0; j2 < 19; j2++)
                        {
                            if ((chess[i1,j1] == chess[i2,j2]) && !((i1 == i2) && (j1 == j2)) && chess[i1,j1] != 0)
                            {
                                p1.x = j1; p1.y = i1; p2.x = j2; p2.y = i2;
                                if (check2p(p1, p2))
                                {
                                    click2p(p1, p2);
                                    return;
                                }
                            }
                        }
                }
            }
        }

        public static void removeAll(int gameBase)//秒杀，清除所有棋子
        {
            //读出棋盘数据至ChessData 11,19
            if (!updateChess(gameBase)) return;
            //遍历整个棋盘 找出相同类型的一对棋子
            Point p1, p2;
            int i1, i2, j1, j2;
            Start:
            for (i1 = 0; i1 < 11; i1++)
            {
                for (j1 = 0; j1 < 19; j1++)
                {
                    for (i2 = 0; i2 < 11; i2++)
                        for (j2 = 0; j2 < 19; j2++)
                        {
                            if (chess[i1, j1] != 0 && (chess[i1, j1] == chess[i2, j2]) && !((i1 == i2) && (j1 == j2)))
                            {
                                p1.x = j1; p1.y = i1; p2.x = j2; p2.y = i2;
                                if (check2p(p1, p2))
                                {
                                    click2p(p1, p2);
                                    chess[i1, j1] = 0;
                                    chess[i2, j2] = 0;
                                    goto Start;
                                }
                            }
                        }
                }
            }
        }
    }
}
